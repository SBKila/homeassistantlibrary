#pragma once

#include <Arduino.h>
#include <Arduino.h>
#include "HAComponent.hpp"

namespace HALIB_NAMESPACE
{

    class HAComponentSensor : public HAComponent
    {
    public:
        HAComponentSensor(const char *name, DeviceClass pDeviceClass, boolean isRetain) : HAComponent(name, SENSOR)
        {
            char *deviceClass = HAUtils::strdup_P(DeviceClassValue[pDeviceClass]);
            addProperty(PROP_DEVICE_CLASS, deviceClass);
            free(deviceClass);
            addProperty(PROP_STATE_TOPIC);
        }
        virtual void onHAConnect()
        {
            if (!m_isRetain)
            {
                publishState();
            }
        }
        void setValue(float value)
        {
            if (m_value != value)
            {
                m_value = value;
                publishState();
            }
        }

    protected:
        void publishState()
        {
            char topic[getTopicPath(PROP_STATE_TOPIC) + 1];
            getTopicPath(PROP_STATE_TOPIC, topic);

            if (NULL != m_pNode)
            {
                char buffer[13];
                sprintf(buffer, "%10.1f", m_value);
                m_pNode->postMessage(topic, buffer, m_isRetain);
            }
            else
            {
                HALIB_COMPONENT_DEBUG_MSG("Node not configured\n");
            }
        };

    private:
        float m_value;
        boolean m_isRetain = false;
    };

} // namespace HALIB_NAMESPACE