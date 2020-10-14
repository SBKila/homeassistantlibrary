#pragma once
#include <Arduino.h>
#include "HAComponent.hpp"

namespace HALIB_NAMESPACE
{
    class HAComponentLigthBasic : public HAComponent
    {
    public:
        HAComponentLigthBasic(const char *pName, HA_BASICLIGHT_CALLBACK callback) : HAComponent(pName, LIGHT)
        {
            m_stateON = false;
            addProperty(PROP_STATE_TOPIC);
            HAComponentPropertyAction<HA_BASICLIGHT_CALLBACK> *l_pAction = new HAComponentPropertyAction<HA_BASICLIGHT_CALLBACK>(PROP_COMMAND_TOPIC, callback);
            addAction(l_pAction);
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
        void _executeAction(HAComponentProperty *p_pAction, const byte *payload, const unsigned int length)
        {
            if (length == 2)
            {
                if (0 == strncmp_P((char *)payload, PropertyDefaultValue[PROP_PAYLOAD_ON], length))
                {
                    HAComponentPropertyAction<HA_BASICLIGHT_CALLBACK> *l_pAction = (HAComponentPropertyAction<HA_BASICLIGHT_CALLBACK> *)p_pAction;
                    l_pAction->getCallback()(true);
                };
            }
            if (length == 3)
            {
                if (0 == strncmp_P((char *)payload, PropertyDefaultValue[PROP_PAYLOAD_OFF], length))
                {
                    HAComponentPropertyAction<HA_BASICLIGHT_CALLBACK> *l_pAction = (HAComponentPropertyAction<HA_BASICLIGHT_CALLBACK> *)p_pAction;
                    l_pAction->getCallback()(false);
                };
            }
        }
        void onHAConnect()
        {
            publishState();
        }

    private:
        void publishState()
        {
            DEBUG_PRINTLN("===>publishState");
            char topic[getTopicPath(PROP_STATE_TOPIC) + 1];
            //DEBUG_PRINTLN_DEC(sizeof(topic));
            getTopicPath(PROP_STATE_TOPIC, topic);
            //DEBUG_PRINTLN(topic);

            // const char* message =  getProperty(m_stateON?PROP_PAYLOAD_ON:PROP_PAYLOAD_OFF,true);
            // DEBUG_PRINTLN(message);

            if (NULL != m_pNode)
            {
                m_pNode->postMessage(topic, m_stateON ? "ON" : "OFF", true);
            }
            else
            {
                DEBUG_PRINTLN("Node not configured");
            }
        }

        boolean m_stateON;
    };
} // namespace HALIB_NAMESPACE