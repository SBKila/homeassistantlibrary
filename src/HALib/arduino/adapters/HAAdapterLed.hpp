#pragma once

#include "./HAAdapter.hpp"
#include "../../protocol/components/HAComponentLigthBasic.hpp"

namespace HALIB_NAMESPACE
{
    class HAAdapterLed : public HAAdapter
    {
    public:
        HAAdapterLed(const char *name, uint8_t ioReference) : HAAdapter(name, ioReference)
        {
            m_pComponent = new HAComponentLigthBasic(name, [this](boolean powerOn) { this->powerOn(powerOn); });
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
            pinMode(m_IOReference, OUTPUT);
            digitalWrite(m_IOReference, HIGH);
        }

    private:
#ifdef UNIT_TEST
    public:
#endif
        HAComponentLigthBasic *m_pComponent;
        virtual void powerOn(boolean powerOn)
        {
            digitalWrite(m_IOReference, powerOn ? LOW : HIGH);
        };
    };
} // namespace HALIB_NAMESPACE
