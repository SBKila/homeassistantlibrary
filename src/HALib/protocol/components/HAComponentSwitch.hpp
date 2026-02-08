#pragma once

#include <Arduino.h>
#include <Arduino.h>
#include "HAComponent.h"

namespace HALIB_NAMESPACE
{
    class HAComponentSwitch : public HAComponent
    {
    public:
        HAComponentSwitch(const char *name) : HAComponent(name, SWITCH)
        {
            m_stateON = false;
            addProperty(PROP_STATE_TOPIC);
        }
        void setState(boolean pStateON)
        {
            m_stateON = pStateON;
            publishState();
        }
        boolean getState()
        {
            return m_stateON;
        }

    protected:
        void _executeAction(HAComponentProperty *p_pAction, byte *payload, unsigned int length) {};
        void onHAConnect()
        {
            publishState();
        }

    private:
        void publishState()
        {
            char topic[getTopicPath(PROP_STATE_TOPIC) + 1];
            getTopicPath(PROP_STATE_TOPIC, topic);

            if (NULL != m_pNode)
            {
                m_pNode->postMessage(topic, m_stateON ? getPropertyByKey(PROP_PAYLOAD_ON) : getPropertyByKey(PROP_PAYLOAD_OFF), true);
            }
            else
            {
                DEBUG_PRINTLN("Node not configured");
            }
        }
        boolean m_stateON;
    };
} // namespace HALIB_NAMESPACE