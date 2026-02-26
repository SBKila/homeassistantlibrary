#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <AsyncMqttClient.h>

// Adjust paths according to your project structure
#include "../../protocol/node/HANode.h"
#include "../../protocol/components/HAComponent.h"
#include "../../protocol/HAMessage.h"     // Required for HAMessage usage
#include "../../protocol/HASubscribCmd.h" // Required for HASubscribCmd usage

namespace HALIB_NAMESPACE
{
    class HADevice
    {
    public:
        // Constructor
        HADevice(const char *pDeviceName, const char *pManuf = NULL, const char *pModel = NULL, const char *pRelease = NULL);

        // Destructor
        ~HADevice();

        // Initializes the MQTT client and networking
        void setup(const char *brokerMqttUrl, const int brokerMqttPort, const char *brokerMqttLogin = NULL, const char *brokerMqttPwd = NULL);

        // Checks if MQTT is connected
        bool isMqttconnected();

        // Sets the MQTT username
        void setMqttUser(const char *user);

        // Sets the MQTT password
        void setMqttPassword(const char *pass);

        // Main loop function, handles WiFi and MQTT maintenance
        void loop(wl_status_t wifiStatus);

        // Returns the internal Node object
        HANode *getNode();

        // Removes a component from the device
        void removeComponent(HAComponent *p_pComponent);

        // Adds a component to the device
        void addComponent(HAComponent *p_pComponent);

        // Returns the device name
        const char *getName();

        // Callbacks AsyncMqtt
        void _onMqttConnect(bool sessionPresent);
        void _onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
        void _onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);

    private:
#ifdef UNIT_TEST
    public:
#endif
        long lastConnectAttempt = 0;
        boolean isSetup = false;
        boolean hasBeenConnected = false;

        AsyncMqttClient m_MqttClient;
        HANode *m_pNode;
        wl_status_t m_PreviousWifiStatus = WL_IDLE_STATUS;
        wl_status_t m_PreviousOpenHabMqttStatus = WL_DISCONNECTED;
        ESP8266WiFiClass *mWifi = &WiFi;
        HAComponent *m_pRecentltyAddedComponent = NULL;

        char *m_brokerMqttLogin = NULL;
        char *m_brokerMqttPwd = NULL;
        char *m_generatedDeviceId = NULL;

        LinkedList<HAComponentProperty *> mProperties;

        // Connects to the MQTT broker
        void connectMQTTServer(boolean cleanSession);

        // Sends a message (HAMessage) to MQTT
        bool postMessage(HAMessage *message);

        // Subscribes or unsubscribes from a topic
        bool subscribeTopic(HASubscribCmd *p_pCmd);

        // Processes all pending actions in the queue
        void treatActions();

        // Processes a single action
        bool treatAction(HAAction *l_pCmd);

        // Sends the device availability ("online"/"offline")
        bool sendAvailability(boolean available);
    };
} // namespace HALIB_NAMESPACE