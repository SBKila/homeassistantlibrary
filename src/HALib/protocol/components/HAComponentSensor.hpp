#pragma once

#include <Arduino.h>
#include <Arduino.h>
#include "HAComponent.h"
#include "HADeviceClass.hpp"
#include "../../tools/debug.h"
namespace HALIB_NAMESPACE
{

    class HAComponentSensor : public HAComponent
    {
    public:
        HAComponentSensor(const char *name, uint8_t pDeviceClass, boolean isRetain) : HAComponent(name, SENSOR)
        {
            HALIB_COMPONENT_DEBUG_MSG("Constructor %s,%s,%d\n", __func__, name, pDeviceClass);
            PGM_P deviceClassPtr = (PGM_P)pgm_read_ptr(&SensorDeviceClassValue[pDeviceClass]);
            HALIB_COMPONENT_DEBUG_MSG("=> device class: \"%s\"\n", deviceClassPtr);
            addProperty(PROP_DEVICE_CLASS, deviceClassPtr);
            HALIB_COMPONENT_DEBUG_MSG("=> state topic:\n");
            addProperty(PROP_STATE_TOPIC, NULL);
            m_isRetain = isRetain;
            m_value = 0.0f;
            HALIB_COMPONENT_DEBUG_MSG("Constructor END\n");
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
            HALIB_COMPONENT_DEBUG_MSG("Publishing state: %10.1f\n", m_value);
            char topic[getTopicPath(PROP_STATE_TOPIC) + 1];
            getTopicPath(PROP_STATE_TOPIC, topic);

            if (NULL != m_pNode)
            {
                char buffer[64];
                snprintf(buffer, sizeof(buffer), "%10.1f", m_value);
                m_pNode->postMessage(topic, buffer, m_isRetain);
            }
            else
            {
                HALIB_COMPONENT_DEBUG_MSG("Node not configured\n");
            }
            HALIB_COMPONENT_DEBUG_MSG("%sEND\n", __func__);
        };

    private:
        float m_value;
        boolean m_isRetain = false;
    };

} // namespace HALIB_NAMESPACE