#pragma once

#include <Arduino.h>
#include "HAComponent.h"

namespace HALIB_NAMESPACE
{
    class HAComponentBinarySensor : public HAComponent
    {
    public:
        HAComponentBinarySensor(const char *name, DeviceClass pDeviceClass = DC_NOTDEFINED) : HAComponent(name, BINARY_SENSOR)
        {
            if (DC_NOTDEFINED != pDeviceClass)
            {
                PGM_P deviceClassPtr = (PGM_P)pgm_read_ptr(&DeviceClassValue[pDeviceClass]);
                addProperty(PROP_DEVICE_CLASS, deviceClassPtr);
            }
            addProperty(PROP_STATE_TOPIC);
        }
        void onHAConnect()
        {
            publishState();
        }
        void setState(boolean pStateON)
        {
            m_stateON = pStateON;
            publishState();
        }
        void publishState()
        {
            char topic[getTopicPath(PROP_STATE_TOPIC) + 1];
            getTopicPath(PROP_STATE_TOPIC, topic);

            if (NULL != m_pNode)
            {
                m_pNode->postMessage(topic, m_stateON ? getPropertyValue(PROP_PAYLOAD_ON) : getPropertyValue(PROP_PAYLOAD_OFF), true);
            }
            else
            {
                HALIB_COMPONENT_DEBUG_MSG("Node not configured\n");
            }
        }

    private:
        boolean m_stateON;
    };
} // namespace HALIB_NAMESPACE