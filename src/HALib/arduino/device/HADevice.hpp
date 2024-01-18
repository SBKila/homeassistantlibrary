#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <avr/pgmspace.h>
#include <PubSubClient.h>

#include "../../protocol/node/HANode.hpp"
#include "../../protocol/components/HAComponent.hpp"

namespace HALIB_NAMESPACE
{
    const char DEVICEIDENTIFIER_TEMPALTE[] PROGMEM = "%s-%08X";
    class HADevice
    {

    public:
        HADevice(const char *pDeviceName, const char *pManuf = NULL, const char *pModel = NULL, const char *pRelease = NULL)
        {
            HALIB_DEVICE_DEBUG_MSG("Constructor\n");
            mWifi = &WiFi;
            m_pNode = new HANode(pDeviceName);
            m_pNode->setDeviceInfo(pManuf, pModel, pRelease);
            m_brokerMqttLogin = NULL;
            m_brokerMqttPwd = NULL;
            HALIB_DEVICE_DEBUG_MSG("ConstructorEND\n");
        };
        ~HADevice()
        {
            HALIB_DEVICE_DEBUG_MSG("Destructor\n");
            delete m_pNode;

            if (NULL != m_brokerMqttLogin)
            {
                free(m_brokerMqttLogin);
            }
            if (NULL != m_brokerMqttPwd)
            {
                free(m_brokerMqttPwd);
            }
            HAUtils::deleteProperties(mProperties);
            HALIB_DEVICE_DEBUG_MSG("DestructorEND\n");
        };

        void setup(Client &pEthClient, const char *brokerMqttUrl, const int brokerMqttPort, const char *brokerMqttLogin = NULL, const char *brokerMqttPwd = NULL)
        {
            HALIB_DEVICE_DEBUG_MSG("Setup\n");

            // disconnect before changing settings
            if (isMqttconnected())
            {
                m_MqttClient.disconnect();
            }

            m_MqttClient.setClient(pEthClient);

            m_MqttClient.setServer(brokerMqttUrl, brokerMqttPort);
            HALIB_DEVICE_DEBUG_MSG("MQtt Domain: %s\n", brokerMqttUrl);
            HALIB_DEVICE_DEBUG_MSG("MQtt Port: %d\n", brokerMqttPort);

            setMqttUser(brokerMqttLogin);
            setMqttPassword(brokerMqttPwd);
            HALIB_DEVICE_DEBUG_MSG("Login: %s\n", m_brokerMqttLogin);
            HALIB_DEVICE_DEBUG_MSG("Pwd: %s\n", m_brokerMqttPwd);

            m_MqttClient.setCallback([this](char *topic, byte *payload, unsigned int length)
                                     { this->onMQTTMessage(topic, payload, length); });

            isSetup = true;
            HALIB_DEVICE_DEBUG_MSG("SetupEND\n");
        };
        bool isMqttconnected()
        {
            return m_MqttClient.connected();
        }
        void setMqttUser(const char *user)
        {

            if (NULL != m_brokerMqttLogin)
            {
                free(m_brokerMqttLogin);
                m_brokerMqttLogin = NULL;
            }
            if (user)
            {
                m_brokerMqttLogin = strdup(user);
            }
        }
        void setMqttPassword(const char *pass)
        {
            if (NULL != m_brokerMqttPwd)
            {
                free(m_brokerMqttPwd);
                m_brokerMqttPwd = NULL;
            }
            if (pass)
            {
                m_brokerMqttPwd = strdup(pass);
            }
        }

        unsigned long lastConnect = 0;
        void loop(wl_status_t wifiStatus)
        {
            if (!isSetup)
                return;
            if (m_PreviousWifiStatus != wifiStatus)
            { // wifi just connected
                HALIB_DEVICE_DEBUG_MSG("wifi status changed %d->%d\n", m_PreviousWifiStatus, wifiStatus);
            }
            // wl_status_t status = mWifi->status();
            if (wifiStatus == WL_CONNECTED)
            {
                if (!m_MqttClient.connected())
                {   // mqtt not connected
                    // retry every second
                    
                    unsigned long delta = millis() - lastConnect;
                    if ((delta > 10000))
                    {
                        // HALIB_DEVICE_DEBUG_MSG("%d\n", delta);
                        if (connectMQTTServer(lastConnect!=0))
                        {
                            HALIB_DEVICE_DEBUG_MSG("=> mqtt now connected\n");
                            sendAvailability(true);
                            m_pNode->onHAConnect(true);
                        }
                        else
                        { // unable to connect mqtt server
                            m_pNode->onHAConnect(false);
                            /* @TODO error management back to setup mode ?*/
                        }
                    }
                }
                else
                { // mqtt still connected
                    // DEBUG_PRINTLN("=> mqtt connected");
                    lastConnect = millis();
                    treatActions();
                }
                // if new component added, resent OnConnect
                if (NULL != m_pRecentltyAddedComponent)
                {
                    getNode()->onHAConnect(true);
                    m_pRecentltyAddedComponent = NULL;
                }
                m_MqttClient.loop();
            }
            m_PreviousWifiStatus = wifiStatus;
        };
        HANode *getNode()
        {
            return m_pNode;
        }
        void removeComponent(HAComponent *p_pComponent)
        {
            HALIB_DEVICE_DEBUG_MSG("removeComponent\n");
            m_pNode->removeComponent(p_pComponent);
            HALIB_DEVICE_DEBUG_MSG("removeComponentEND\n");
        }
        void addComponent(HAComponent *p_pComponent)
        {
            HALIB_DEVICE_DEBUG_MSG("addComponent\n");
            if (NULL == m_pNode)
            {
                HALIB_DEVICE_DEBUG_MSG("addComponent\n");
            }
            m_pNode->addComponent(p_pComponent);
            HALIB_DEVICE_DEBUG_MSG("addComponent2\n");
            if (m_PreviousWifiStatus == WL_CONNECTED)
            {
                m_pRecentltyAddedComponent = p_pComponent;
                HALIB_DEVICE_DEBUG_MSG("addComponent2\n");
                //     p_pComponent->_onHAConnect();
                HALIB_DEVICE_DEBUG_MSG("addComponent3\n");
            }
            HALIB_DEVICE_DEBUG_MSG("addComponentEND\n");
        }
        const char *getName()
        {
            return m_pNode->getName();
        }

    private:
#ifdef UNIT_TEST
    public:
#endif
        boolean isSetup = false;
        PubSubClient m_MqttClient;
        HANode *m_pNode;
        wl_status_t m_PreviousWifiStatus = WL_IDLE_STATUS;
        wl_status_t m_PreviousOpenHabMqttStatus = WL_DISCONNECTED;
        ESP8266WiFiClass *mWifi = &WiFi;
        HAComponent *m_pRecentltyAddedComponent = NULL;

        void onMQTTMessage(char *topic, unsigned char *payload, unsigned int length)
        {
            HALIB_DEVICE_DEBUG_MSG("onMQTTMessage\n");
            m_pNode->onHAMessage(topic, payload, length);
        }

        bool connectMQTTServer(boolean cleanSession)
        {
            uint32_t identifier = ESP.getChipId() ^ ESP.getFlashChipId();
            const char *nodeName = m_pNode->getName();

            HALIB_DEVICE_DEBUG_MSG("mqtt connecting with :\n");
            // <node_name>-identifier
            int deviceIdLength = strlen(nodeName) + 1 + 8 + 1;
            char *deviceId = (char *)calloc(deviceIdLength, sizeof(char));
            // apply template
            sprintf_P(deviceId, DEVICEIDENTIFIER_TEMPALTE, nodeName, identifier);
            HALIB_DEVICE_DEBUG_MSG("deviceId: %s\n", deviceId);

            const char *willTopic = m_pNode->getProperty(PROP_AVAILABILITY_TOPIC);

            uint8_t willQos = 0;
            boolean willRetain = true;
            const char *willMessage = m_pNode->getProperty(PROP_PAYLOAD_NOT_AVAILABLE);
            HALIB_DEVICE_DEBUG_MSG("willTopic: %s\n", willTopic);
            HALIB_DEVICE_DEBUG_MSG("willQos: %d\n", willQos);
            HALIB_DEVICE_DEBUG_MSG("willRetain: %s\n", (willRetain) ? "TRUE" : "FALSE");
            HALIB_DEVICE_DEBUG_MSG("willMessage: %s\n", willMessage);
            HALIB_DEVICE_DEBUG_MSG("Login: %s\n", m_brokerMqttLogin);
            HALIB_DEVICE_DEBUG_MSG("Pwd: %s\n", m_brokerMqttPwd);

            m_MqttClient.disconnect(); //@TODO Really needed ?
            bool status = m_MqttClient.connect(deviceId, m_brokerMqttLogin, m_brokerMqttPwd, willTopic, willQos, willRetain, willMessage, cleanSession);
            HALIB_DEVICE_DEBUG_MSG("mqtt connection status: %s\n", (status) ? "true" : "false");

            free(deviceId);
            return status;
        }
        bool postMessage(HAMessage *message)
        {
            HALIB_DEVICE_DEBUG_MSG("postMessage\n");
            const char *offset = message->getMessage();
            const char *topic = message->getTopic();

            size_t messageLength = strlen(offset);
            // HALIB_DEVICE_DEBUG_MSG("postMessage messageLength(%d) offset(%s)\n",messageLength,offset);

            boolean success = false;

            if (MQTT_MAX_PACKET_SIZE < (messageLength + 2 + strlen(topic)))
            {
                // HALIB_DEVICE_DEBUG_MSG("Long Message\n");
                success = m_MqttClient.beginPublish(topic, messageLength, 0);
                // HALIB_DEVICE_DEBUG_MSG("starting seding %d",messageLength);
                while (success && (0 != messageLength))
                {
                    int sentdata = 0;
                    sentdata = m_MqttClient.write((byte *)offset, messageLength);
                    success = 0 != sentdata;
                    offset += sentdata;
                    messageLength = strlen(offset);
                    // HALIB_DEVICE_DEBUG_MSG("sentdata %d",messageLength);
                }
                // HALIB_DEVICE_DEBUG_MSG("ending\n");
                m_MqttClient.endPublish();
                // HALIB_DEVICE_DEBUG_MSG("sent\n");
            }
            else
            {
                // HALIB_DEVICE_DEBUG_MSG("Short Message\n");
                success = m_MqttClient.publish(topic, offset);
            }
            HALIB_DEVICE_DEBUG_MSG("postMessageEND (%s)\n", (success) ? "true" : "false");
            return success;
        }
        bool subscribeTopic(HASubscribCmd *p_pCmd)
        {
            boolean success = false;
            if (p_pCmd->isSubscribtion())
            {
                HALIB_DEVICE_DEBUG_MSG("Subscription to %s\n", p_pCmd->getTopic());
                success = m_MqttClient.subscribe(p_pCmd->getTopic());
            }
            else
            {
                HALIB_DEVICE_DEBUG_MSG("Unsubscription to %s\n", p_pCmd->getTopic());
                success = m_MqttClient.unsubscribe(p_pCmd->getTopic());
            }
            return success;
        }

        void treatActions()
        {
            int nbAction = m_pNode->actionsSize();
            while (
                (0 != nbAction--) &&
                treatAction(m_pNode->pickupAction()))
                ;
        }
        bool treatAction(HAAction *l_pCmd)
        {
            boolean success = false;
            HALIB_DEVICE_DEBUG_MSG("treatAction\n");

            if (NULL != l_pCmd)
            {
                // HALIB_DEVICE_DEBUG_MSG("treatMessage type %d",l_pCmd->getType());
                boolean done = false;
                switch (l_pCmd->getType())
                {
                case POSTMESSAGE:
                    done = postMessage((HAMessage *)l_pCmd);
                    break;
                case SUBSCRIPTION:
                    done = subscribeTopic((HASubscribCmd *)l_pCmd);
                    break;
                }

                if (done)
                {
                    delete l_pCmd;
                }
                else
                {
                    if (l_pCmd->retry() < 250)
                    {
                        m_pNode->retryAction(l_pCmd);
                    }
                    else
                    {
                        delete l_pCmd;
                    }
                }
                success = true;
            }

            HALIB_DEVICE_DEBUG_MSG("treatActionEND (%s)\n", (success) ? "true" : "false");
            return success;
        }
        bool sendAvailability(boolean available)
        {
            HALIB_DEVICE_DEBUG_MSG("sendAvailability\n");
            const char *availabilityTopic = m_pNode->getProperty(PROP_AVAILABILITY_TOPIC);
            const char *availabilityMessage = m_pNode->getProperty(available ? PROP_PAYLOAD_AVAILABLE : PROP_PAYLOAD_NOT_AVAILABLE);
            HALIB_DEVICE_DEBUG_MSG("Topic: %s\n", availabilityTopic);
            HALIB_DEVICE_DEBUG_MSG("Message: %s\n", availabilityMessage);
            boolean status = m_MqttClient.publish(availabilityTopic, availabilityMessage, true);
            HALIB_DEVICE_DEBUG_MSG("sendAvailabilityEND\n");
            return status;
        }

        char *m_brokerMqttLogin;
        char *m_brokerMqttPwd;
        LinkedList<HAComponentProperty *> mProperties;
    };
} // namespace HALIB_NAMESPACE