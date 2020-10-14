#pragma once
#include <Arduino.h>
#include "HAComponent.hpp"

namespace HALIB_NAMESPACE
{
    class HAComponentDeviceTrigger : public HAComponent
    {
    public:
        HAComponentDeviceTrigger(const char *name);
        void trigEvent();
        virtual void onHAConnect();

    protected:
        void _executeAction(HAComponentProperty * p_pAction, byte * payload, unsigned int length){};
    };

    HAComponentDeviceTrigger::HAComponentDeviceTrigger(const char *name) : HAComponent(name, DEVICE_TRIGGER)
    {
        addProperty(PROP_AUTOMATION_TYPE, "trigger");
        addProperty(PROP_TOPIC_TOPIC);
        addProperty(PROP_TYPE, "button_short_press");
        addProperty(PROP_STYPE, "button1");
    }

    void HAComponentDeviceTrigger::onHAConnect()
    {
    }
    
    void HAComponentDeviceTrigger::trigEvent()
    {
        char topic[getTopicPath(PROP_TOPIC_TOPIC) + 1];
        getTopicPath(PROP_TOPIC_TOPIC, topic);
        if (NULL != m_pNode)
        {
            m_pNode->postMessage(topic, "1", false);
        }
        else
        {
            DEBUG_PRINTLN("Node not configured");
        }
    }
} // namespace HALIB_NAMESPACE