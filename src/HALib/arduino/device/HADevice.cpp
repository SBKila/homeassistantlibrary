#include "HADevice.h"
#include <avr/pgmspace.h>
#include "../../tools/debug.h"
#include "../../tools/HAUtils.h"

namespace HALIB_NAMESPACE
{
    // Moved to CPP to avoid multiple definition errors
    const char DEVICEIDENTIFIER_TEMPLATE[] PROGMEM = "%s-%08X";

    HADevice::HADevice(const char *pDeviceName, const char *pManuf, const char *pModel, const char *pRelease)
    {
        HALIB_DEVICE_DEBUG_MSG("Constructor\n");
        mWifi = &WiFi;
        m_pNode = new HANode(pDeviceName);
        m_pNode->setDeviceInfo(pManuf, pModel, pRelease);
        m_brokerMqttLogin = NULL;
        m_brokerMqttPwd = NULL;
        HALIB_DEVICE_DEBUG_MSG("ConstructorEND\n");
    }

    HADevice::~HADevice()
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
    }

    void HADevice::setup(Client &pEthClient, const char *brokerMqttUrl, const int brokerMqttPort, const char *brokerMqttLogin, const char *brokerMqttPwd)
    {
        HALIB_DEVICE_DEBUG_MSG("Setup\n");

        // Disconnect before changing settings
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
    }

    bool HADevice::isMqttconnected()
    {
        return m_MqttClient.connected();
    }

    void HADevice::setMqttUser(const char *user)
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

    void HADevice::setMqttPassword(const char *pass)
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

    void HADevice::loop(wl_status_t wifiStatus)
    {
        if (!isSetup)
            return;
        if (m_PreviousWifiStatus != wifiStatus)
        { // Wifi just connected or changed state
            HALIB_DEVICE_DEBUG_MSG("wifi status changed %d->%d\n", m_PreviousWifiStatus, wifiStatus);
        }

        if (wifiStatus == WL_CONNECTED)
        {
            if (!m_MqttClient.connected())
            {
                // MQTT not connected, retry every 10 seconds
                unsigned long delta = millis() - lastConnect;
                if ((delta > 10000))
                {
                    if (connectMQTTServer(lastConnect != 0))
                    {
                        HALIB_DEVICE_DEBUG_MSG("=> mqtt reconnected\n");
                        sendAvailability(true);
                        m_pNode->onHAConnect(true);
                    }
                    else
                    {
                        // Unable to connect to MQTT server
                        /* @TODO error management back to setup mode ?*/
                        m_pNode->onHAConnect(false);
                    }
                    lastConnect = millis();
                }
            }
            else
            {
                // MQTT still connected
                lastConnect = millis();
                treatActions();

                // If new component added, resend OnConnect
                if (NULL != m_pRecentltyAddedComponent)
                {
                    m_pNode->onHAConnect(m_MqttClient.connected());
                    m_pRecentltyAddedComponent = NULL;
                }
            }
            m_MqttClient.loop();
        }
        m_PreviousWifiStatus = wifiStatus;
    }

    HANode *HADevice::getNode()
    {
        return m_pNode;
    }

    void HADevice::removeComponent(HAComponent *p_pComponent)
    {
        HALIB_DEVICE_DEBUG_MSG("removeComponent\n");
        m_pNode->removeComponent(p_pComponent);
        HALIB_DEVICE_DEBUG_MSG("removeComponentEND\n");
    }

    void HADevice::addComponent(HAComponent *p_pComponent)
    {
        HALIB_DEVICE_DEBUG_MSG("addComponent\n");
        if (NULL != m_pNode)
        {
            m_pNode->addComponent(p_pComponent);
            if (m_PreviousWifiStatus == WL_CONNECTED)
            {
                m_pRecentltyAddedComponent = p_pComponent;
            }
        }
        HALIB_DEVICE_DEBUG_MSG("addComponentEND\n");
    }

    const char *HADevice::getName()
    {
        return m_pNode->getName();
    }

    void HADevice::onMQTTMessage(char *topic, unsigned char *payload, unsigned int length)
    {
        HALIB_DEVICE_DEBUG_MSG("onMQTTMessage\n");
        m_pNode->onHAMessage(topic, payload, length);
    }

    bool HADevice::connectMQTTServer(boolean cleanSession)
    {
        uint32_t identifier = ESP.getChipId() ^ ESP.getFlashChipId();
        const char *nodeName = m_pNode->getName();

        HALIB_DEVICE_DEBUG_MSG("mqtt connecting with :\n");
        // <node_name>-identifier
        int deviceIdLength = strlen(nodeName) + 1 + 8 + 1;
        char *deviceId = (char *)calloc(deviceIdLength, sizeof(char));

        // Apply template
        sprintf_P(deviceId, DEVICEIDENTIFIER_TEMPLATE, nodeName, identifier);
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

    bool HADevice::postMessage(HAMessage *message)
    {
        HALIB_DEVICE_DEBUG_MSG("postMessage\n");
        boolean success = false;
        if (m_MqttClient.connected())
        {
            const char *offset = message->getMessage();
            const char *topic = message->getTopic();

            size_t messageLength = strlen(offset);

            if (MQTT_MAX_PACKET_SIZE < (messageLength + 2 + strlen(topic)))
            {
                // Message too long, using beginPublish/write/endPublish
                success = m_MqttClient.beginPublish(topic, messageLength, 0);
                while (success && (0 != messageLength))
                {
                    int sentdata = 0;
                    sentdata = m_MqttClient.write((byte *)offset, messageLength);
                    success = 0 != sentdata;
                    offset += sentdata;
                    messageLength = strlen(offset);
                }
                m_MqttClient.endPublish();
            }
            else
            {
                // Short Message, standard publish
                success = m_MqttClient.publish(topic, offset);
            }
        }
        HALIB_DEVICE_DEBUG_MSG("postMessageEND (%s)\n", (success) ? "true" : "false");
        return success;
    }

    bool HADevice::subscribeTopic(HASubscribCmd *p_pCmd)
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

    void HADevice::treatActions()
    {
        int nbAction = m_pNode->actionsSize();
        // Process all pending actions
        while ((0 != nbAction--) && treatAction(m_pNode->pickupAction()))
            ;
    }

    bool HADevice::treatAction(HAAction *l_pCmd)
    {
        boolean success = false;
        HALIB_DEVICE_DEBUG_MSG("treatAction\n");

        if (NULL != l_pCmd)
        {
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
                // If action failed, retry unless counter exceeded
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

    bool HADevice::sendAvailability(boolean available)
    {
        HALIB_DEVICE_DEBUG_MSG("sendAvailability\n");
        const char *availabilityTopic = m_pNode->getProperty(PROP_AVAILABILITY_TOPIC);
        const char *availabilityMessage = m_pNode->getProperty(available ? PROP_PAYLOAD_AVAILABLE : PROP_PAYLOAD_NOT_AVAILABLE);
        HALIB_DEVICE_DEBUG_MSG("Topic: %s\n", availabilityTopic);
        HALIB_DEVICE_DEBUG_MSG("Message: %s\n", availabilityMessage);

        m_MqttClient.publish(availabilityTopic, availabilityMessage, true);

        HALIB_DEVICE_DEBUG_MSG("sendAvailabilityEND\n");
        return true;
    }

} // namespace HALIB_NAMESPACE