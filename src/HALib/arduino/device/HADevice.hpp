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
            mWifi = &WiFi;
            m_pNode = new HANode(pDeviceName);
            m_pNode->setDeviceInfo(pManuf, pModel, pRelease);
            m_openHabMqttUrl = NULL;
        };
        ~HADevice()
        {
            delete m_pNode;

            if (NULL != m_openHabMqttUrl)
            {
                free(m_openHabMqttUrl);
            }

            HAUtils::deleteProperties(mProperties);
        };

        void setup(Client &pEthClient, const char *openHabMqttUrl, const int openHabMqttport)
        {
            m_MqttClient.setClient(pEthClient);
            m_MqttClient.setServer(openHabMqttUrl, openHabMqttport);

            m_openHabMqttUrl = strdup(openHabMqttUrl);
            m_openHabMqttport = openHabMqttport;

            m_MqttClient.setCallback([this](char *topic, byte *payload, unsigned int length) { this->onMQTTMessage(topic, payload, length); });
        };

        

        void loop(wl_status_t wifiStatus)
        {
            //wl_status_t status = mWifi->status();
            if (wifiStatus == WL_CONNECTED)
            { // assigned when wifi connection is established
                //DEBUG_PRINTLN("=> Wifi connected");
                // if (m_PreviousWifiStatus != WL_CONNECTED)
                // { // wifi just connected
                //     // DEBUG_PRINTLN("wifi just connected");
                // }
                // else
                { // wifi connected
                    if (!m_MqttClient.connected())
                    { // mqtt not connected
                        //DEBUG_PRINTLN("=> mqtt not connected");
                        //m_MqttClient.loop();
                        if (connectMQTTServer())
                        {
                            DEBUG_PRINTLN("=> mqtt now connected");
                            sendAvailability(true);
                            m_pNode->onHAConnect();
                        }
                        else
                        { // unable to connect mqtt server
                            /* @todo error management back to setup mode ?*/
                        }
                    }
                    else
                    { // mqtt still connected
                        //DEBUG_PRINTLN("=> mqtt connected");
                        //m_MqttClient.loop();
                        while(treatActions());
                    }

                    m_MqttClient.loop();
                }
            }
            m_PreviousWifiStatus = wifiStatus;
        };
        HANode *getNode()
        {
            return m_pNode;
        }
        void addComponent(HAComponent *p_pComponent)
        {
            m_pNode->addComponent(p_pComponent);
            if(m_PreviousWifiStatus==WL_CONNECTED){
                p_pComponent->_onHAConnect();
            }
        }
    private:
#ifdef UNIT_TEST
    public:
#endif
        PubSubClient m_MqttClient;
        HANode *m_pNode;
        wl_status_t m_PreviousWifiStatus = WL_DISCONNECTED;
        wl_status_t m_PreviousOpenHabMqttStatus = WL_DISCONNECTED;
        ESP8266WiFiClass *mWifi = &WiFi;

        
        void onMQTTMessage(char *topic, unsigned char *payload, unsigned int length)
        {
            //DEBUG_PRINTLN("onMQTTMessage");
            m_pNode->onHAMessage(topic, payload, length);
        }

        bool connectMQTTServer()
        {
            uint32_t identifier = ESP.getChipId() ^ ESP.getFlashChipId();
            const char *nodeName = m_pNode->getName();

            //DEBUG_PRINTLN("mqtt connecting with :");
            // <node_name>-identifier
            int deviceIdLength = strlen(nodeName) + 1 + 8 + 1;
            char *deviceId = (char *)calloc(deviceIdLength, sizeof(char));
            // apply template
            sprintf_P(deviceId, DEVICEIDENTIFIER_TEMPALTE, nodeName, identifier);
            //DEBUG_PRINT("deviceId:");
            //DEBUG_PRINT(deviceId);
            //DEBUG_PRINTLN("");

            const char *willTopic = m_pNode->getProperty(PROP_AVAILABILITY_TOPIC);
            ;
            uint8_t willQos = 0;
            boolean willRetain = true;
            const char *willMessage = m_pNode->getProperty(PROP_PAYLOAD_NOT_AVAILABLE);
            // DEBUG_PRINT("willTopic:");
            // DEBUG_PRINT(willTopic);
            // DEBUG_PRINTLN("");
            // DEBUG_PRINT("willQos:");
            // DEBUG_PRINTLN_DEC(willQos);
            // DEBUG_PRINT("willRetain:");
            // DEBUG_PRINT(willRetain ? "TRUE" : "FALSE");
            // DEBUG_PRINTLN("");
            // DEBUG_PRINT("willMessage:");
            // DEBUG_PRINT(willMessage);
            // DEBUG_PRINTLN("");

            bool status = m_MqttClient.connect(deviceId, willTopic, willQos, willRetain, willMessage);
            //@todo login/password will message

            // DEBUG_PRINT(" mqtt connection status = ");
            // DEBUG_PRINTLN(status);

            free(deviceId);

            return status;
        }
        bool postMessage(HAMessage *message)
        {
            const char *offset = message->getMessage();
            const char* topic = message->getTopic();

        size_t messageLength = strlen(offset);
            // DEBUG_PRINT("postMessage ");
            // DEBUG_PRINT(messageLength);
            // DEBUG_PRINT(" ");
            // DEBUG_PRINTLN(offset);
            
            boolean success = false;

            if (MQTT_MAX_PACKET_SIZE < (messageLength+2+strlen(topic)))
            {
                // DEBUG_PRINTLN("Long Message");
                success = m_MqttClient.beginPublish(topic, messageLength, 0);
                // DEBUG_PRINTLN("starting");
                //DEBUG_PRINTLN_DEC(messageLength);
                while (success && (0 != messageLength))
                {
                    int sentdata = 0;
                    sentdata = m_MqttClient.write((byte *)offset, messageLength);
                    success = 0 != sentdata;
                    offset += sentdata;
                    messageLength = strlen(offset);

                    // DEBUG_PRINTLN_DEC(sentdata);
                    // DEBUG_PRINTLN_DEC(messageLength);
                    // DEBUG_PRINTLN("sending");
                }

                // DEBUG_PRINTLN("ending");
                m_MqttClient.endPublish();
                // DEBUG_PRINTLN("sent");
            }
            else
            {
                // DEBUG_PRINTLN("Short Message");
                success = m_MqttClient.publish(topic, offset);
            }

            return success;
        }
        bool subscribeTopic(HASubscribCmd *p_pCmd)
        {
            boolean success = false;
            if (p_pCmd->isSubscribtion())
            {
                // DEBUG_PRINT("Subscription to ");
                // DEBUG_PRINTLN(p_pCmd->getTopic());
                success = m_MqttClient.subscribe(p_pCmd->getTopic());
            }
            else
            {
                // DEBUG_PRINT("Unsubscription to ");
                // DEBUG_PRINTLN(p_pCmd->getTopic());
                success = m_MqttClient.unsubscribe(p_pCmd->getTopic());
            }
            return success;
        }
        bool treatActions()
        {
            //DEBUG_PRINTLN("treatActions");
            HAAction *l_pCmd = m_pNode->pickupOutboxAction();
            if (NULL != l_pCmd)
            {
                // DEBUG_PRINT("treatMessage type");
                // DEBUG_PRINTLN(l_pCmd->getType());
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
                return true;
            }
            else
            {
                return false;
            }
        }
        bool sendAvailability(boolean available)
        {
            const char *availabilityTopic = m_pNode->getProperty(PROP_AVAILABILITY_TOPIC);
            const char *availabilityMessage = m_pNode->getProperty(available ? PROP_PAYLOAD_AVAILABLE : PROP_PAYLOAD_NOT_AVAILABLE);
            // DEBUG_PRINT("Topic:");DEBUG_PRINT(availabilityTopic);DEBUG_PRINTLN("");
            // DEBUG_PRINT("Message:");DEBUG_PRINT(availabilityMessage);DEBUG_PRINTLN("");
            // DEBUG_PRINTLN_DEC(MQTT_MAX_PACKET_SIZE);
            m_MqttClient.publish(availabilityTopic, availabilityMessage, true);
            // DEBUG_PRINTLN("Availability sent");
            return true;
        }

        char *m_openHabMqttUrl;
        int m_openHabMqttport;
        LinkedList<HAComponentProperty *> mProperties;
    };
} // namespace HALIB_NAMESPACE