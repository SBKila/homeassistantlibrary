#pragma once

#include <Arduino.h>
#include "HAComponent.hpp"

namespace HALIB_NAMESPACE
{
    class HAComponentBinarySensor : public HAComponent
    {
    public:
        HAComponentBinarySensor(const char *name, DeviceClass pDeviceClass = DC_NOTDEFINED) : HAComponent(name, BINARY_SENSOR)
        {
            if (DC_NOTDEFINED != pDeviceClass)
            {
                char *deviceClass = HAUtils::strdup_P(DeviceClassValue[pDeviceClass]);
                addProperty(PROP_DEVICE_CLASS, deviceClass);
                free(deviceClass);
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
                DEBUG_PRINTLN("Node not configured");
            }
        }

    private:
        boolean m_stateON;
    };
} // namespace HALIB_NAMESPACE