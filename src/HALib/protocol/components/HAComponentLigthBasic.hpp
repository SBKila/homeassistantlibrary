#pragma once
#include <Arduino.h>
#include "HAComponent.h"
#include "../../arduino/HACallbackFunction.h"

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
                PGM_P expectedPayload = (PGM_P)pgm_read_ptr(&PropertyDefaultValue[PROP_PAYLOAD_ON]);
                if (0 == strncmp_P((char *)payload, expectedPayload, length))
                {
                    HAComponentPropertyAction<HA_BASICLIGHT_CALLBACK> *l_pAction = (HAComponentPropertyAction<HA_BASICLIGHT_CALLBACK> *)p_pAction;
                    l_pAction->getCallback()(true);
                };
            }
            if (length == 3)
            {
                PGM_P expectedPayload = (PGM_P)pgm_read_ptr(&PropertyDefaultValue[PROP_PAYLOAD_OFF]);
                if (0 == strncmp_P((char *)payload, expectedPayload, length))
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
            HALIB_COMPONENT_DEBUG_MSG("===>publishState\n");
            char topic[getTopicPath(PROP_STATE_TOPIC) + 1];
            // HALIB_COMPONENT_DEBUG_MSG_DEC(sizeof(topic));
            getTopicPath(PROP_STATE_TOPIC, topic);
            // HALIB_COMPONENT_DEBUG_MSG(topic);

            // const char* message =  getProperty(m_stateON?PROP_PAYLOAD_ON:PROP_PAYLOAD_OFF,true);
            // HALIB_COMPONENT_DEBUG_MSG(message);

            if (NULL != m_pNode)
            {
                m_pNode->postMessage(topic, m_stateON ? "ON" : "OFF", true);
            }
            else
            {
                HALIB_COMPONENT_DEBUG_MSG("Node not configured\n");
            }
        }

        boolean m_stateON;
    };
} // namespace HALIB_NAMESPACE