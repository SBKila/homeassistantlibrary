#pragma once

#include "./HAAdapter.h"
#include "../../protocol/components/HAComponentDeviceTrigger.h"
#include <EasyButton.h>
#include "../../tools/debug.h"
namespace HALIB_NAMESPACE
{
    class HAAdapterPushBtn : public HAAdapter
    {
    public:
        HAAdapterPushBtn(const char *name, uint8_t ioReference) : HAAdapter(name, ioReference), m_Button(ioReference)
        {
            m_pComponent = new HAComponentDeviceTrigger(name);
        }
        virtual ~HAAdapterPushBtn()
        {
            m_Button.disableInterrupt();
            delete (m_pComponent);
        }
        virtual void _setup()
        {
            // DEBUG_PRINT(F("Setup button "));
            // DEBUG_PRINTLN(m_pComponent->getName());

            if (m_Button.supportsInterrupt())
            {
                // DEBUG_PRINTLN("supports Interrupt");
                m_Button.enableInterrupt([this]()
                                         { HALIB_COMPONENT_DEBUG_MSG("===>");this->onBtPressed(); });
            }
            else
            {
                // DEBUG_PRINTLN("dont' supports Interrupt");
                m_Button.onPressed([this]()
                                   { HALIB_COMPONENT_DEBUG_MSG("===>");this->onBtPressed(); });
            }

            m_Button.begin();
        }
        virtual void suspend(boolean state)
        {
            if (m_Button.supportsInterrupt())
            {

                if (state)
                {
                    m_Button.disableInterrupt();
                }
                else
                {
                    m_Button.enableInterrupt([this]()
                                             {HALIB_COMPONENT_DEBUG_MSG("===>");this->onBtPressed(); });
                }
            }
        }
        virtual void onBtPressed()
        {
            // DEBUG_PRINT(F("On bt pressed "));
            // DEBUG_PRINT(m_pComponent->getName());
            m_pComponent->trigEvent();
        }
        // virtual JsonObject toJson()
        // {        {
        //     const size_t capacity = JSON_OBJECT_SIZE(5);
        //     StaticJsonDocument<capacity> doc;
        //     JsonObject object = doc.to<JsonObject>();

        //     object["type"] = "PushButton";
        //     object["name"] = m_Name;
        //     object["ref"] = m_IOReference;

        //     return object;
        // }

        // }
        // void onHAConnect();

        virtual void setDevice(HADevice *p_pDevice)
        {
            p_pDevice->addComponent(m_pComponent);
        }

    protected:
        HAComponentDeviceTrigger *m_pComponent;
        uint8_t m_DigitalIO;
        EasyButton m_Button;
    };
} // namespace HALIB_NAMESPACE