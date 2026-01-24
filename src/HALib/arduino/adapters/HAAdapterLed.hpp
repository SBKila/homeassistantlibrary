#pragma once

#include "./HAAdapter.h"
#include "../../protocol/components/HAComponentLigthBasic.hpp"

namespace HALIB_NAMESPACE
{
    class HAAdapterLed : public HAAdapter
    {
    public:
        HAAdapterLed(const char *name, uint8_t ioReference) : HAAdapter(name, ioReference)
        {
            m_pComponent = new HAComponentLigthBasic(name, [this](boolean powerOn)
                                                     { this->powerOn(powerOn); });
        }
        ~HAAdapterLed()
        {
            delete (m_pComponent);
        }
        virtual void setDevice(HADevice *p_pDevice)
        {
            p_pDevice->addComponent(m_pComponent);
        }
        virtual void _setup()
        {
            pinMode(m_pinNumber, OUTPUT);
            digitalWrite(m_pinNumber, HIGH);
        }

    private:
#ifdef UNIT_TEST
    public:
#endif
        HAComponentLigthBasic *m_pComponent;
        virtual void powerOn(boolean powerOn)
        {
            digitalWrite(m_pinNumber, powerOn ? LOW : HIGH);
        };
    };
} // namespace HALIB_NAMESPACE
