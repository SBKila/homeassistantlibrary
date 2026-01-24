#pragma once

#include <Arduino.h>
#include "IHAAdapter.h"
#include "../../protocol/components/HAComponentDeviceTrigger.h"
#include <EasyButton.h>

namespace HALIB_NAMESPACE
{
    class HAAdapter : public IHAAdapter
    {
    public:
        // Constructeur
        HAAdapter(const char *name, uint8_t ioReference);

        // Destructeur virtuel
        virtual ~HAAdapter();

        // Getters
        virtual const char *getName() const;

        // Méthodes de cycle de vie
        virtual void _setup() = 0; // Reste purement virtuelle
        virtual void setup();
        virtual void loop();
        virtual void suspend(boolean state);

        // Opérateurs
        virtual bool operator!=(const IHAAdapter &other);
        virtual bool operator==(const IHAAdapter &other);

    protected:
        char m_Name[20];
        uint8_t m_pinNumber;
    };
} // namespace HALIB_NAMESPACE