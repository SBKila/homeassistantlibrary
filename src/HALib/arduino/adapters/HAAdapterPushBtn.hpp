#pragma once

#include "./HAAdapter.h"
#include "../../protocol/components/HAComponentDeviceTrigger.h"
#include "../../tools/debug.h"

namespace HALIB_NAMESPACE
{
    class HAAdapterPushBtn : public HAAdapter
    {
    public:
        HAAdapterPushBtn(const char *name, uint8_t ioReference) : HAAdapter(name, ioReference)
        {
            m_pComponent = new HAComponentDeviceTrigger(name);
            m_DigitalIO = ioReference;
            m_ButtonState = HIGH;
            m_LastState = HIGH;
            m_LastDebounceTime = 0;
            m_DebounceDelay = 50; // 50ms default for debounce
        }
        
        virtual ~HAAdapterPushBtn()
        {
            delete (m_pComponent);
        }
        
        virtual void _setup()
        {
            pinMode(m_DigitalIO, INPUT_PULLUP);
            m_LastState = digitalRead(m_DigitalIO);
            m_ButtonState = m_LastState;
        }
        
        virtual void _loop()
        {
            int reading = digitalRead(m_DigitalIO);

            if (reading != m_LastState)
            {
                m_LastDebounceTime = millis();
            }

            if ((millis() - m_LastDebounceTime) > m_DebounceDelay)
            {
                if (reading != m_ButtonState)
                {
                    m_ButtonState = reading;
                    if (m_ButtonState == LOW) // Button press detected
                    {
                        HALIB_COMPONENT_DEBUG_MSG("===>\n");
                        onBtPressed();
                    }
                }
            }
            m_LastState = reading;
        }

        virtual void suspend(boolean state)
        {
            // Ignored in native polling mode
        }

        virtual void onBtPressed()
        {
            m_pComponent->trigEvent();
        }

        virtual void setDevice(HADevice *p_pDevice)
        {
            p_pDevice->addComponent(m_pComponent);
        }

    protected:
        HAComponentDeviceTrigger *m_pComponent;
        uint8_t m_DigitalIO;
        int m_ButtonState;
        int m_LastState;
        unsigned long m_LastDebounceTime;
        unsigned long m_DebounceDelay;
    };
} // namespace HALIB_NAMESPACE