#pragma once

#include <arduinoJson.h>
#include "HAAdapter.hpp"
#include "../../protocol/components/HAComponentSensor.hpp"

#define HA_DDS238_PERSISTENT_FUNCTION std::function<void(DDS238Data data)>

namespace HALIB_NAMESPACE
{
    struct DDS238Data
    {
        int tag;
        uint ticks;
        float cumulative;
    };

    class HAAdapterDDS238 : public HAAdapter
    {
    public:
        HAAdapterDDS238(const char *name, uint8_t ioReference, ushort tickByKW, ushort voltage, ushort maxAmp, HA_DDS238_PERSISTENT_FUNCTION persistentFunction) : HAAdapter(name, ioReference)
        {

            m_TickByTenthKW = tickByKW / 10;
            m_PersistenceFunction = persistentFunction;

            // reset persistant data
            m_Persistent.tag = 963;
            m_Persistent.ticks = 0;
            m_Persistent.cumulative = 0;

            m_VolatileTickTime = 0;
            m_VolatileInterupDownTime = 0;
            m_VolatileTicks = 0;
            debugToShortDelay = 0;
            m_VolatileState = HIGH;
            m_VolatileAverageTickDuration = 0;

            size_t nameLength = strlen(name);
            char *buffer = (char *)calloc(nameLength + 8 + 1, sizeof(char));

            // Setup HA sensor for cumulative power
            strcpy(buffer, name);
            strcat(buffer, "_cumulat");
            m_pCumulaticComponent = new HAComponentSensor(buffer,DC_ENERGY, false);
            m_pCumulaticComponent->addProperty(PROP_UNIT_OF_MEASUREMENT, "kWh");
            m_pCumulaticComponent->addProperty(PROP_STATE_CLASS, "total_increasing");

            // Setup HA sensor for instant power
            strcpy(buffer, name);
            strcat(buffer, "_instant");
            m_pInstantComponent = new HAComponentSensor(buffer, DC_ENERGY, false);
            m_pInstantComponent->addProperty(PROP_UNIT_OF_MEASUREMENT, "kWh");
            m_pInstantComponent->addProperty(PROP_STATE_CLASS, "measurement");

            free(buffer);

            m_InstantFactor = 3600000 / tickByKW;
            m_MinDuration = 3600000. / ((voltage * maxAmp) * (1000. / tickByKW));

            debugTotalTicks = 0;
            debugToShort = 0;
            debugLastTickDuration = 0;
        }
        virtual ~HAAdapterDDS238()
        {
            suspend(true);

            delete m_pCumulaticComponent;
            delete m_pInstantComponent;
        }
        virtual void setDevice(HADevice *p_pDevice)
        {
            p_pDevice->addComponent(m_pCumulaticComponent);
            p_pDevice->addComponent(m_pInstantComponent);
        }

        virtual void restore(DDS238Data persistence)
        {
            // restore only if value ok
            if (persistence.tag == 963)
            {
                m_Persistent = persistence;

                // send value to MQTT
                m_pCumulaticComponent->setValue(m_Persistent.cumulative / 10);

                debugTotalTicks = 0;
                debugToShort = 0;
                debugToShortDelay = 0;
            } else {
                // persist cumulative power
                m_PersistenceFunction(m_Persistent);
            }
        }
        virtual void loop()
        {
            // if some tick happend since last loop
            if (m_VolatileTicks > 0)
            {
                int nbTick = 0;
                unsigned long tickTime = 0;
                noInterrupts();
                nbTick = m_VolatileTicks;
                m_VolatileTicks = 0;
                tickTime = m_VolatileTickTime;
                interrupts();

                // add tick to persistent counter
                m_Persistent.ticks += nbTick;

                // if number of ticks is bigger than a tenth of Kw
                if (m_Persistent.ticks > m_TickByTenthKW)
                {
                    // increment cumulative power
                    m_Persistent.cumulative++;

                    // decrement ticks
                    m_Persistent.ticks -= m_TickByTenthKW;

                    // send value to MQTT
                    m_pCumulaticComponent->setValue(m_Persistent.cumulative / 10);

                    // persist cumulative power
                    m_PersistenceFunction(m_Persistent);
                }

                // Instant power management
                if (0 != m_LastTickTreatedTime) // skip first tick
                {
                    unsigned long deltaT = tickTime - m_LastTickTreatedTime;

                    // if(deltaT<14000)
                    // {
                    //     debugToShort++;
                    // }
                    // if(deltaT>16000)
                    // {
                    //     debugToLong++;
                    // }
                    if (deltaT < 0) // overflow millis() return to 0
                    {
                        deltaT = 4294967295 - m_LastTickTreatedTime;
                        deltaT += tickTime;
                    }
                    if (deltaT > 0)
                    {
                        m_InstantPower = nbTick * m_InstantFactor / deltaT;
                        m_pInstantComponent->setValue(m_InstantPower);
                    }
                    m_LastTickDeltaTime = deltaT;
                }
                m_LastTickTreatedTime = tickTime;
                // just for blink led no data store as far data not changed
                m_PersistenceFunction(m_Persistent);
            }
            else
            {
                if (m_InstantPower != 0)
                {
                    if ((millis() - m_LastTickTreatedTime) > (10 * m_InstantFactor))
                    {
                        m_InstantPower = 0;
                        m_pInstantComponent->setValue(0);
                    }
                }
            }
        }
        virtual void _setup()
        {
            // setup pin as input with pull up resistor
            pinMode(m_IOReference, INPUT_PULLUP);
        }
        virtual void suspend(boolean state)
        {
            // setup callback on power meter signal
            if (state)
            {
                detachInterrupt(digitalPinToInterrupt(m_IOReference));
            }
            else
            {
                attachInterrupt(
                    digitalPinToInterrupt(m_IOReference), [this]() {
                        noInterrupts();
                        this->onTick();
                        interrupts();
                    },
                    CHANGE);
            }
        }

        virtual void onTick()
        {
            unsigned long now = millis();
            // if (m_VolatileInterupTime != 0)
            // {
            m_VolatileState = digitalRead(m_IOReference);
            if (HIGH == m_VolatileState)
            {
                // Raising use case

                if (0 != m_VolatileInterupDownTime) // if previous did failling taken in account
                {
                    unsigned long delta = now - m_VolatileInterupDownTime;
                    debugLastTickDuration = delta;

                    if (delta >= 30) // duration min is 30ms According to EN 62053-31
                    {
                        // this signal should be taken in acount
                        if (m_VolatileAverageTickDuration == 0)
                        {
                            m_VolatileAverageTickDuration = delta;
                        }
                        else
                        {
                            m_VolatileAverageTickDuration = (m_VolatileAverageTickDuration + delta) / 2;
                        }

                        m_VolatileTicks++;
                        m_VolatileTickTime = now;
                        debugTotalTicks++;
                    }
                    else
                    {
                        debugToShort++;
                    }

                    // ignore this tick
                    m_VolatileInterupDownTime = 0;
                }
                m_VolatileInterupUpTime = now;
            }
            else
            {
                // Falling use case
                if (0 != m_VolatileInterupUpTime)
                { // normal Usecase
                    unsigned long delta = now - m_VolatileInterupUpTime;
                    if (delta >= 30) // duration min is 30ms According to EN 62053-31
                    {
                        // check if delay between tick is correct according to max power
                        if ((now - delta) > m_MinDuration)
                        {
                            m_VolatileInterupDownTime = now;
                        }
                        else
                        {
                            debugToShortDelay++;
                            m_VolatileInterupDownTime = 0;
                        }
                    }
                    else
                    {
                        debugToShortDelay++;
                        m_VolatileInterupDownTime = 0;
                    }
                }
                else
                { // First tick or previous ignored
                    m_VolatileInterupDownTime = now;
                }
            }
            // }
            // else
            // {
            //     // synchro count with FALLING
            //     if (LOW == digitalRead(m_DigitalIO))
            //     {
            //         m_VolatileInterupTime = now;
            //         m_VolatileState = LOW;
            //     }
            //     else
            //     {
            //         m_VolatileState = HIGH;
            //     }
            // }
        }

        virtual JsonObject toJson()
        {
            const size_t capacity = JSON_OBJECT_SIZE(5);
            StaticJsonDocument<capacity> doc;
            JsonObject object = doc.to<JsonObject>();

            object["type"] = "DDS238";
            object["name"] = m_Name;
            object["ref"] = m_IOReference;
            object["tickbyKW"] = m_TickByTenthKW * 10;
            object["voltage"] = 220;
            object["amp"] = 16;

            return object;
        }

/*
static const char htmlDebugPatern[] PROGMEM = "\
<div><div>Ticks <span>Total:%lu </span>\
<span>Current:%d </span>\
<span>ToShort:%d </span>\
<span>ToClose:%d </span>\
</div><div>Duration \
<span>Last:%d </span>\
<span>Average:%.2f </span>\
<span>Since last:%lu </span>\
</div><div>Power\
<span>instant:%.2f </span>\
<span>Tick/TenthKW:%d </span>\
</div><div>\
state:%s\
</div><div>\
minDuration=%u\
 </div>";
*/
        //         void toHtmlDebug(char *output)
        //         {
        //             sprintf_P(output, htmlDebugPatern,
        //                       debugTotalTicks,
        //                       m_Persistent.ticks,
        //                       debugToShort,
        //                       debugToShortDelay,
        //                       debugLastTickDuration,
        //                       m_VolatileAverageTickDuration,

        //                       m_LastTickDeltaTime,
        //                       m_InstantPower,
        //                       m_TickByTenthKW,
        //                       (HIGH == m_VolatileState) ? "HIGH" : "LOW",
        //                       m_MinDuration);
        //         }

    protected:
        ushort m_TickByTenthKW;
        unsigned int m_MinDuration;

        HA_DDS238_PERSISTENT_FUNCTION m_PersistenceFunction;

        HAComponentSensor *m_pCumulaticComponent;
        HAComponentSensor *m_pInstantComponent;

        DDS238Data m_Persistent;
        unsigned long m_VolatileTickTime;
        unsigned long m_VolatileInterupUpTime;
        unsigned long m_VolatileInterupDownTime;
        unsigned short m_VolatileState;
        double m_VolatileAverageTickDuration;
        boolean m_VolatileStandbyState;
        int m_VolatileTicks;

        unsigned long debugTotalTicks;
        int debugToShort;
        int debugToShortDelay;
        int debugLastTickDuration;

        boolean m_IsTicked;

        float m_InstantFactor;
        unsigned long m_LastTickTreatedTime = 0;
        unsigned long m_LastTickDeltaTime = 0;
        float m_InstantPower = 0;

        //String getPMReference(uint8_t digitalIO);
    };
} // namespace HALIB_NAMESPACE