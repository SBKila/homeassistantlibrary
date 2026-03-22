#pragma once

#include <Arduino.h>
#include <string.h>
#include "HAComponent.h"
#include "HADeviceClass.hpp"
#include "../../tools/debug.h"

namespace HALIB_NAMESPACE
{

    class HAComponentTextSensor : public HAComponent
    {
    public:
        HAComponentTextSensor(const char *name, uint8_t pDeviceClass = SC_NONE, boolean isRetain = false) : HAComponent(name, SENSOR)
        {
            HALIB_COMPONENT_DEBUG_MSG("Constructor %s,%s,%d\n", __func__, name, pDeviceClass);
            PGM_P deviceClassPtr = (PGM_P)pgm_read_ptr(&SensorDeviceClassValue[pDeviceClass]);
            HALIB_COMPONENT_DEBUG_MSG("=> device class: \"%s\"\n", deviceClassPtr);
            addProperty(PROP_DEVICE_CLASS, deviceClassPtr);
            HALIB_COMPONENT_DEBUG_MSG("=> state topic:\n");
            addProperty(PROP_STATE_TOPIC, NULL);
            m_isRetain = isRetain;
            memset(m_value, 0, sizeof(m_value));
            HALIB_COMPONENT_DEBUG_MSG("Constructor END\n");
        }

        virtual void onHAConnect()
        {
            if (!m_isRetain)
            {
                publishState();
            }
        }

        void setValue(const char *value)
        {
            if (strncmp(m_value, value, sizeof(m_value) - 1) != 0)
            {
                strncpy(m_value, value, sizeof(m_value) - 1);
                m_value[sizeof(m_value) - 1] = '\0'; // Ensure null-termination
                publishState();
            }
        }

        void setValue(const String &value)
        {
            setValue(value.c_str());
        }

    protected:
        void publishState()
        {
            HALIB_COMPONENT_DEBUG_MSG("Publishing state: %s\n", m_value);
            char topic[getTopicPath(PROP_STATE_TOPIC) + 1];
            getTopicPath(PROP_STATE_TOPIC, topic);

            if (NULL != m_pNode)
            {
                m_pNode->postMessage(topic, m_value, m_isRetain);
            }
            else
            {
                HALIB_COMPONENT_DEBUG_MSG("Node not configured\n");
            }
            HALIB_COMPONENT_DEBUG_MSG("%sEND\n", __func__);
        };

    private:
        char m_value[128]; // Buffer to store the current text state
        boolean m_isRetain = false;
    };

} // namespace HALIB_NAMESPACE
