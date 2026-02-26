#include "HADevice.h"
#include <avr/pgmspace.h>
#include "../../tools/debug.h"
#include "../../tools/HAUtils.h"

#define WILLQOS 0

namespace HALIB_NAMESPACE
{
    // Moved to CPP to avoid multiple definition errors
    const char DEVICEIDENTIFIER_TEMPLATE[] PROGMEM = "%s-%08X";

    HADevice::HADevice(const char *pDeviceName, const char *pManuf, const char *pModel, const char *pRelease)
    {
        HALIB_DEVICE_DEBUG_MSG("Constructor %s %s\n", __func__, pDeviceName);
        mWifi = &WiFi;
        m_pNode = new HANode(pDeviceName);
        m_pNode->setDeviceInfo(pManuf, pModel, pRelease);
        m_brokerMqttLogin = NULL;
        m_brokerMqttPwd = NULL;

        // Generate Home Assistant Device Id
        uint32_t identifier = ESP.getChipId() ^ ESP.getFlashChipId();
        const char *nodeName = m_pNode->getName();
        m_generatedDeviceId = (char *)calloc(strlen(nodeName) + 10, sizeof(char));
        sprintf_P(m_generatedDeviceId, DEVICEIDENTIFIER_TEMPLATE, nodeName, identifier);

        m_MqttClient.setClientId(m_generatedDeviceId);

        // attach MQTT Async callback
        m_MqttClient.onConnect([this](bool sp)
                               { this->_onMqttConnect(sp); });
        m_MqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason r)
                                  { this->_onMqttDisconnect(r); });
        m_MqttClient.onMessage([this](char *t, char *p, AsyncMqttClientMessageProperties prop, size_t len, size_t i, size_t tot)
                               { this->_onMqttMessage(t, p, prop, len, i, tot); });

        HALIB_DEVICE_DEBUG_MSG("ConstructorEND\n");
    }

    HADevice::~HADevice()
    {
        HALIB_DEVICE_DEBUG_MSG("Destructor\n");
        delete m_pNode;

        if (NULL != m_brokerMqttLogin)
            free(m_brokerMqttLogin);

        if (m_brokerMqttPwd)
            free(m_brokerMqttPwd);

        if (m_generatedDeviceId)
            free(m_generatedDeviceId);

        HAUtils::deleteProperties(mProperties);
        HALIB_DEVICE_DEBUG_MSG("DestructorEND\n");
    }

    void HADevice::setup(const char *brokerMqttUrl, const int brokerMqttPort, const char *brokerMqttLogin, const char *brokerMqttPwd)
    {
        HALIB_DEVICE_DEBUG_MSG("Setup\n");

        m_MqttClient.setServer(brokerMqttUrl, brokerMqttPort);
        HALIB_DEVICE_DEBUG_MSG("MQtt Domain: %s\n", m_brokerMqttPwd ? m_brokerMqttPwd : "NULL");
        HALIB_DEVICE_DEBUG_MSG("MQtt Port: %d\n", brokerMqttPort);
        setMqttUser(brokerMqttLogin);
        setMqttPassword(brokerMqttPwd);
        HALIB_DEVICE_DEBUG_MSG("Login: %s\n", m_brokerMqttLogin ? m_brokerMqttLogin : "NULL");
        HALIB_DEVICE_DEBUG_MSG("Pwd: %s\n", m_brokerMqttPwd ? m_brokerMqttPwd : "NULL");

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
                unsigned long now = millis();
                if (now - lastConnectAttempt > 10000)
                {
                    lastConnectAttempt = now;
                    connectMQTTServer(!hasBeenConnected);
                }
            }
            else
            {
                // MQTT still connected
                treatActions();

                // If new component added, resend OnConnect
                if (NULL != m_pRecentltyAddedComponent)
                {
                    m_pNode->onHAConnect(m_MqttClient.connected());
                    m_pRecentltyAddedComponent = NULL;
                }
            }
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
    void HADevice::_onMqttConnect(bool sessionPresent)
    {
        HALIB_DEVICE_DEBUG_MSG("=> Mqtt connected\n");
        sendAvailability(true);
        m_pNode->onHAConnect(true);
        hasBeenConnected = true;
    }

    void HADevice::_onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
    {
        HALIB_DEVICE_DEBUG_MSG("=> Mqtt disconnected (reason: %d)\n", (int)reason);
        m_pNode->onHAConnect(false);
    }

    void HADevice::_onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
    {
        HALIB_DEVICE_DEBUG_MSG("onMQTTMessage\n");
        m_pNode->onHAMessage(topic, (uint8_t *)payload, len);
    }

    void HADevice::connectMQTTServer(boolean cleanSession)
    {
        HALIB_DEVICE_DEBUG_MSG("Mqtt connecting...\n");

        // Configuration du LWT (Last Will and Testament)
        const char *willTopic = m_pNode->getProperty(PROP_AVAILABILITY_TOPIC);
        const char *willMessage = m_pNode->getProperty(PROP_PAYLOAD_NOT_AVAILABLE);

        if (willTopic && willMessage)
        {

            boolean willRetain = true;
            m_MqttClient.setWill(willTopic, WILLQOS, willRetain, willMessage);
            HALIB_DEVICE_DEBUG_MSG("willTopic: %s\n", willTopic ? willTopic : "NULL");
            HALIB_DEVICE_DEBUG_MSG("willQos: %d\n", WILLQOS);
            HALIB_DEVICE_DEBUG_MSG("willRetain: %s\n", (willRetain) ? "TRUE" : "FALSE");
            HALIB_DEVICE_DEBUG_MSG("willMessage: %s\n", willMessage ? willMessage : "NULL");
        }

        if (m_brokerMqttLogin && m_brokerMqttPwd)
        {
            m_MqttClient.setCredentials(m_brokerMqttLogin, m_brokerMqttPwd);
            HALIB_DEVICE_DEBUG_MSG("Login: %s\n", m_brokerMqttLogin ? m_brokerMqttLogin : "NULL");
            HALIB_DEVICE_DEBUG_MSG("Pwd: %s\n", m_brokerMqttPwd ? m_brokerMqttPwd : "NULL");
        }

        m_MqttClient.setCleanSession(cleanSession);
        m_MqttClient.connect();
    }

    bool HADevice::postMessage(HAMessage *message)
    {
        HALIB_DEVICE_DEBUG_MSG("postMessage\n");
        boolean success = false;
        if (m_MqttClient.connected())
        {
            success = (m_MqttClient.publish(
                           message->getTopic(),
                           0,
                           false,
                           message->getMessage()) > 0);
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
            success = (m_MqttClient.subscribe(p_pCmd->getTopic(), WILLQOS) > 0);
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
        boolean success = false;
        HALIB_DEVICE_DEBUG_MSG("sendAvailability\n");
        const char *availabilityTopic = m_pNode->getProperty(PROP_AVAILABILITY_TOPIC);
        const char *availabilityMessage = m_pNode->getProperty(available ? PROP_PAYLOAD_AVAILABLE : PROP_PAYLOAD_NOT_AVAILABLE);
        HALIB_DEVICE_DEBUG_MSG("Topic: %s\n", availabilityTopic);
        HALIB_DEVICE_DEBUG_MSG("Message: %s\n", availabilityMessage);

        success = (m_MqttClient.publish(availabilityTopic, WILLQOS, true, availabilityMessage) > 0);

        HALIB_DEVICE_DEBUG_MSG("sendAvailabilityEND (%s)\n", (success) ? "true" : "false");
        return true;
    }

} // namespace HALIB_NAMESPACE