#pragma once

#include <Arduino.h>
#include "../../tools/LinkedList.hpp"
#include "../IHAComponent.h"
#include "HAComponentType.h"
#include "HAComponentProperty.h"
#include "HAComponentPropertyAction.hpp"
// Note: We include the base Property class.
// Specific Action/Sensor properties are usually only needed in the implementation or by the user.

namespace HALIB_NAMESPACE
{
    // Forward declaration to avoid circular dependencies
    class IHANode;

    class HAComponent : public IHAComponent
    {
    public:
        // Constructor: Creates a component with a name and a type (e.g., SENSOR, SWITCH)
        HAComponent(const char *pName, ComponentType pComponentType);

        // Destructor: Cleans up properties and actions
        virtual ~HAComponent();

        // Returns the name of the component (stored in properties)
        const char *getName();

        // Returns the unique 32-bit ID of the component
        virtual uint32_t getId();

        // Links this component to a parent Node (device)
        virtual void setNode(IHANode *pNnode);

        // Builds the MQTT topic for Home Assistant discovery
        virtual char *buildDiscoveryTopic(const char *pDiscoveryPrefix, const char *pNodeId = NULL);

        // Builds the JSON payload for Home Assistant discovery
        virtual char *buildDiscoveryMessage();

        // Handles incoming MQTT messages targeted at this component
        virtual bool onHAMessage(const char *topic, const byte *p_pPayload, const unsigned int length);

        // Internal lifecycle method called when connected to HA
        virtual void _onHAConnect();

        // User-overridable method called when connected to HA
        virtual void onHAConnect();

        // Comparison operators based on Component ID
        virtual bool operator==(IHAComponent &other);
        virtual bool operator!=(IHAComponent &other);

        // Adds an action (callback) to the component
        void addAction(HAComponentProperty *p_pProperty);

        // Adds a property to the component (e.g., state topic, unique_id)
        HAComponentProperty *addProperty(HAComponentPropertyKey pName, const char *pValue = NULL);

        // Methods exposed publicly only for Unit Testing
#ifdef UNIT_TEST
    public:
#endif
        // Helper to build the root MQTT topic string
        char *buildRootTopic(const char *pNodeId);

        // Helper to construct the full path of a topic (handling relative paths like "~")
        int getTopicPath(const char *topic, char *out = NULL);

        // Helper to get the root topic property
        const char *getRootTopic();

    protected:
        // Members exposed publicy only for Unit Testing
#ifdef UNIT_TEST
    public:
#endif
        IHANode *m_pNode;

        // Overload to get topic path directly from a property key
        int getTopicPath(HAComponentPropertyKey pName, char *out = NULL);

        // Helper to retrieve a property value by its key
        const char *getPropertyValue(HAComponentPropertyKey name, boolean fallbackDefault = false);

        // Virtual method to execute an action (to be implemented by subclasses if needed)
        virtual void _executeAction(HAComponentProperty *p_pAction, const byte *payload, const unsigned int length);

    private:
        ComponentType m_ComponentType;
        uint32_t m_ComponentId;
        LinkedList<HAComponentProperty *> mProperties;
        LinkedList<HAComponentProperty *> mActions;
        boolean isFirstHAConnection = true;
    };
} // namespace HALIB_NAMESPACE