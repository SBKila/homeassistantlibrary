#pragma once

#include <Arduino.h>
#include "IHAAdapter.hpp"
#include "../../protocol/components/HAComponentDeviceTrigger.hpp"
#include <EasyButton.h>

namespace HALIB_NAMESPACE
{
    class HAAdapter : public IHAAdapter
    {
    public:
        HAAdapter(const char *name, uint8_t ioReference)
        {
            strncpy(m_Name, name, 20);
            //m_Name[19] = NULL;
            m_IOReference = ioReference;
        }
        virtual ~HAAdapter(){
        }

        virtual const char *getName() const
        {
            const char *b = &m_Name[0];

            return b;
        }

        virtual void _setup() = 0;
        virtual void setup()
        {
            _setup();
            suspend(false);
        }

        virtual bool operator!=(const IHAAdapter &other)
        {
            return (strcmp(other.getName(), m_Name) != 0);
        }
        virtual bool operator==(const IHAAdapter &other)
        {
            return (strcmp(other.getName(), m_Name) == 0);
        }

        virtual void loop()
        {
        }
        
        virtual void suspend(boolean state)
        {
        }
    protected:
        char m_Name[20];
        uint8_t m_IOReference;
    };
} // namespace HALIB_NAMESPACE