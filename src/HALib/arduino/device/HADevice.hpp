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
            m_openHabMqttUrl = NULL;
            HALIB_DEVICE_DEBUG_MSG("ConstructorEND\n");
        };
        ~HADevice()
        {
            HALIB_DEVICE_DEBUG_MSG("Destructor\n");
            delete m_pNode;

            if (NULL != m_openHabMqttUrl)
            {
                free(m_openHabMqttUrl);
            }

            HAUtils::deleteProperties(mProperties);
            HALIB_DEVICE_DEBUG_MSG("DestructorEND\n");
        };

        void setup(Client &pEthClient, const char *openHabMqttUrl, const int openHabMqttport)
        {
            HALIB_DEVICE_DEBUG_MSG("Setup\n");
            m_MqttClient.setClient(pEthClient);
            m_MqttClient.setServer(openHabMqttUrl, openHabMqttport);

            m_openHabMqttUrl = strdup(openHabMqttUrl);
            m_openHabMqttport = openHabMqttport;

            m_MqttClient.setCallback([this](char *topic, byte *payload, unsigned int length) { this->onMQTTMessage(topic, payload, length); });
            HALIB_DEVICE_DEBUG_MSG("SetupEND\n");
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
                            HALIB_DEVICE_DEBUG_MSG("=> mqtt now connected\n");
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
                        treatActions();
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
            HALIB_DEVICE_DEBUG_MSG("addComponent\n");
            m_pNode->addComponent(p_pComponent);
            if(m_PreviousWifiStatus==WL_CONNECTED){
                p_pComponent->_onHAConnect();
            }
            HALIB_DEVICE_DEBUG_MSG("addComponentEND\n");
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
            HALIB_DEVICE_DEBUG_MSG("onMQTTMessage\n");
            m_pNode->onHAMessage(topic, payload, length);
        }

        bool connectMQTTServer()
        {
            uint32_t identifier = ESP.getChipId() ^ ESP.getFlashChipId();
            const char *nodeName = m_pNode->getName();

            HALIB_DEVICE_DEBUG_MSG("mqtt connecting with :\n");
            // <node_name>-identifier
            int deviceIdLength = strlen(nodeName) + 1 + 8 + 1;
            char *deviceId = (char *)calloc(deviceIdLength, sizeof(char));
            // apply template
            sprintf_P(deviceId, DEVICEIDENTIFIER_TEMPALTE, nodeName, identifier);
            HALIB_DEVICE_DEBUG_MSG("deviceId: %s\n",deviceId);
            
            const char *willTopic = m_pNode->getProperty(PROP_AVAILABILITY_TOPIC);
            
            uint8_t willQos = 0;
            boolean willRetain = true;
            const char *willMessage = m_pNode->getProperty(PROP_PAYLOAD_NOT_AVAILABLE);
            HALIB_DEVICE_DEBUG_MSG("willTopic: %s\n",willTopic);
            HALIB_DEVICE_DEBUG_MSG("willQos: %d\n",willQos);
            HALIB_DEVICE_DEBUG_MSG("willRetain: %s\n", (willRetain)?"TRUE":"FALSE");
            HALIB_DEVICE_DEBUG_MSG("willMessage: %s\n",willMessage);

            bool status = m_MqttClient.connect(deviceId, willTopic, willQos, willRetain, willMessage);
            //@todo login/password will message

            HALIB_DEVICE_DEBUG_MSG("mqtt connection status: %s\n",status);

            free(deviceId);

            return status;
        }
        bool postMessage(HAMessage *message)
        {
            HALIB_DEVICE_DEBUG_MSG("postMessage\n");
            const char *offset = message->getMessage();
            const char* topic = message->getTopic();

            size_t messageLength = strlen(offset);
            // DEBUG_PRINT("postMessage messageLength(%d) offset(%s)\n",messageLength,offset);
            
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

            HALIB_DEVICE_DEBUG_MSG("postMessageEND (%s)\n",success?"true","false");
            return success;
        }
        bool subscribeTopic(HASubscribCmd *p_pCmd)
        {
            boolean success = false;
            if (p_pCmd->isSubscribtion())
            {
                HALIB_DEVICE_DEBUG_MSG("Subscription to %s\n",p_pCmd->getTopic());
                success = m_MqttClient.subscribe(p_pCmd->getTopic());
            }
            else
            {
                HALIB_DEVICE_DEBUG_MSG("Unsubscription to %s\n",p_pCmd->getTopic());
                success = m_MqttClient.unsubscribe(p_pCmd->getTopic());
            }
            return success;
        }
        
        void treatActions(){
            
            int nbAction = m_pNode->actionsSize();
            
            while(
                (0!=nbAction--) && 
                treatAction(m_pNode->pickupAction()));
            
        }
        bool treatAction(HAAction *l_pCmd)
        {
            boolean success = false;
            HALIB_DEVICE_DEBUG_MSG("treatAction\n");
            
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
                success = true;
            }

            HALIB_DEVICE_DEBUG_MSG("treatActionEND (%s)\n":success?"true":"false");
            return success;
        }
        bool sendAvailability(boolean available)
        {
            const char *availabilityTopic = m_pNode->getProperty(PROP_AVAILABILITY_TOPIC);
            const char *availabilityMessage = m_pNode->getProperty(available ? PROP_PAYLOAD_AVAILABLE : PROP_PAYLOAD_NOT_AVAILABLE);
            HALIB_DEVICE_DEBUG_MSG('sendAvailability\nTopic: %s\nMessage:%s\n',availabilityTopic,availabilityMessage);
            // DEBUG_PRINTLN_DEC(MQTT_MAX_PACKET_SIZE);
            m_MqttClient.publish(availabilityTopic, availabilityMessage, true);
            HALIB_DEVICE_DEBUG_MSG('sendAvailabilityEND\n');
            return true;
        }

        char *m_openHabMqttUrl;
        int m_openHabMqttport;
        LinkedList<HAComponentProperty *> mProperties;
    };
} // namespace HALIB_NAMESPACE