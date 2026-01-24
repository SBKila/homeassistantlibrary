#pragma once

#include <Arduino.h>
#include "../../tools/LinkedList.hpp"
#include "../IHANode.h"
#include "../IHAComponent.h"
#include "../components/HAComponentPropertyConstante.hpp"
// Forward declarations to avoid unnecessary includes in the header
namespace HALIB_NAMESPACE
{
    class HAAction;
    class HAMessage;
    class HAComponentProperty;
}

namespace HALIB_NAMESPACE
{
    class HANode : public IHANode
    {
        // HAComponent needs access to internal members of HANode
        friend class HAComponent;

    public:
        // Constructor: Creates a Node (Device container) with a specific name
        HANode(const char *pName);

        // Destructor: Cleans up memory (name, properties, actions)
        virtual ~HANode();

        // Sets the device information (Manufacturer, Model, Firmware version)
        // This generates the JSON payload for the "device" property in Home Assistant
        void setDeviceInfo(const char *pManuf, const char *pModel, const char *pRelease);

        // --- IHANode Interface Implementation ---

        // Returns the node name
        const char *getName();

        // Returns the unique node ID
        uint32_t getId();

        // Retrieves a property value by its key
        const char *getProperty(HAComponentPropertyKey name);

        // Removes a component from this node
        void removeComponent(IHAComponent *p_pComponent);

        // Adds a component to this node
        void addComponent(IHAComponent *p_pComponent);

        // Sends auto-discovery messages for all attached components to Home Assistant
        void postAutoDiscovery();

        // Sends the discovery message for a specific component
        void postDiscoveryMessage(IHAComponent *pComponent);

        // Handles incoming MQTT messages and dispatches them to the relevant components
        bool onHAMessage(const char *topic, const byte *p_pPayload, const unsigned int length);

        // queues a message to be sent via MQTT (handles retention logic)
        void postMessage(HAMessage *pMessage);

        // Called when the connection to Home Assistant is established or lost
        void onHAConnect(boolean isConnected);

        // Helper to retrieve all discovery messages (used for debugging or specific transports)
        int getDiscoveryMessages(HAMessage *pMessages);

        // Retrieves the next action (message/subscription) from the outbox queue
        HAAction *pickupAction();

        // Returns the number of pending actions in the outbox
        int actionsSize();

        // Re-queues an action that failed to be processed
        void retryAction(HAAction *p_pAction);

    private:
#ifdef UNIT_TEST
    public:
#endif
        // Internal helper to post a raw message
        void postMessage(const char *pTopic, const char *pMessage, boolean retain);

        // Queues a subscription command for a specific topic
        void registerToHA(const char *topic, boolean subscription = true);

    private:
        char *mName;
        uint32_t mId;
        LinkedList<IHAComponent *> mComponents;
        LinkedList<HAAction *> mOutboxAction;
        LinkedList<HAComponentProperty *> mProperties;
        boolean mIsHAConnected;
    };
} // namespace HALIB_NAMESPACE