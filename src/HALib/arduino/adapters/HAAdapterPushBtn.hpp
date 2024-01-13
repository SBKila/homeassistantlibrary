#pragma once

#include "./HAAdapter.hpp"
#include "../../protocol/components/HAComponentDeviceTrigger.hpp"
#include <EasyButton.h>


namespace HALIB_NAMESPACE
{
    class HAAdapterPushBtn : public HAAdapter
    {
    public:
        HAAdapterPushBtn(const char *name, uint8_t ioReference) : HAAdapter(name,ioReference)
        {
            m_pComponent = new HAComponentDeviceTrigger(name);
            m_pButton = new EasyButton(ioReference);
        }
        virtual ~HAAdapterPushBtn()
        {
            m_pButton->disableInterrupt();
            
            delete (m_pComponent);
            delete (m_pButton);
        }
        virtual void _setup()
        {
            // DEBUG_PRINT(F("Setup button "));
            // DEBUG_PRINTLN(m_pComponent->getName());
            
            if (m_pButton->supportsInterrupt())
            {
                //DEBUG_PRINTLN("supports Interrupt");
                m_pButton->enableInterrupt([this]() {DEBUG_PRINT(F("===>"));this->onBtPressed(); });
            }
            else
            {
                //DEBUG_PRINTLN("dont' supports Interrupt");
                m_pButton->onPressed([this]() {DEBUG_PRINT(F("===>"));this->onBtPressed(); });
            }

            m_pButton->begin();
        }
        virtual void suspend(boolean state)
        {
            if (m_pButton->supportsInterrupt()){

                if(state)
                {
                    m_pButton->disableInterrupt();
                }
                else
                {
                    m_pButton->enableInterrupt([this]() {DEBUG_PRINT(F("===>"));this->onBtPressed(); });
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
        //void onHAConnect();

        virtual void setDevice(HADevice *p_pDevice)
        {
            p_pDevice->addComponent(m_pComponent);
        }

    protected:
        HAComponentDeviceTrigger *m_pComponent;
        uint8_t m_DigitalIO;
        EasyButton *m_pButton;
    };
} // namespace HALIB_NAMESPACE