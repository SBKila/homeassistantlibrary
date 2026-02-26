#pragma once

#include "HAAdapter.h"
#include "../../protocol/components/HAComponentSensor.hpp"
#include "../../tools/BufferFIFO.hpp"

#define HA_DDS238_PERSISTENT_FUNCTION std::function<void(DDS238Data data)>

// -------------------------------------------------------------------------
// Architecture Compatibility Helper
// -------------------------------------------------------------------------
#if defined(ESP8266)
#ifndef IRAM_ATTR
#define IRAM_ATTR ICACHE_RAM_ATTR
#endif
#define ISR_PREFIX IRAM_ATTR
#elif defined(ESP32)
#define ISR_PREFIX IRAM_ATTR
#else
#define ISR_PREFIX
#endif

namespace HALIB_NAMESPACE
{
#define DEFAULTTAG 987
#define MAGICTAG 963
    struct DDS238Data
    {
        int tag = DEFAULTTAG;
        uint ticks = 0;
        float cumulative = 0;
    };

    class HAAdapterDDS238 : public HAAdapter
    {
    public:
        HAAdapterDDS238(const char *name, uint8_t ioReference, ushort tickByKW, ushort voltage, ushort maxAmp, HA_DDS238_PERSISTENT_FUNCTION persistentFunction, boolean average) : HAAdapter(name, ioReference)
                                                                                                                                                                                        HAAdapterDDS238(const char *name, uint8_t ioReference, ushort tickByKW, ushort voltage, ushort maxAmp, HA_DDS238_PERSISTENT_FUNCTION persistentFunction, boolean average, int rtcOffset = -1) : HAAdapter(name, ioReference)
        {
            HALIB_ADAPTER_DEBUG_MSG("Constructor\n");

            m_AverageBuffer.initialize(10);

            if (tickByKW == 0)
                tickByKW = 2000;
            if (voltage > 230)
                voltage = 230;
            if ((maxAmp != 32) && (maxAmp != 20) && (maxAmp != 16) && (maxAmp != 10))
                maxAmp = 16;

            m_AverageActive = average;
            m_TickByTenthKW = tickByKW / 10;
            m_rtcOffset = rtcOffset;
            m_PersistenceFunction = persistentFunction;

            // Reset persistent data
            m_Persistent.tag = MAGICTAG;
            m_Persistent.ticks = 0;
            m_Persistent.cumulative = 0;

            // Reset volatile data
            m_lastPulseTimestamp = 0;
            m_pulseStartTime = 0;
            m_pulseCount = 0;
            m_LastTickTreatedTime = 0;

            m_pDevice = NULL;
            size_t nameLength = strlen(name);
            char *buffer = (char *)calloc(nameLength + 8 + 1, sizeof(char));

            // Setup HA sensor for cumulative power
            strcpy(buffer, name);
            strcat(buffer, "_cumulat");
            m_pCumulaticComponent = new HAComponentSensor(buffer, SC_ENERGY, false);
            m_pCumulaticComponent->addProperty(PROP_UNIT_OF_MEASUREMENT, "kWh");
            m_pCumulaticComponent->addProperty(PROP_STATE_CLASS, "total_increasing");

            // Setup HA sensor for instant power
            strcpy(buffer, name);
            strcat(buffer, "_instant");
            m_pInstantComponent = new HAComponentSensor(buffer, SC_POWER, false);
            m_pInstantComponent->addProperty(PROP_UNIT_OF_MEASUREMENT, "kW");
            m_pInstantComponent->addProperty(PROP_STATE_CLASS, "measurement");

            free(buffer);
            m_InstantFactor = 3600000.0f / tickByKW;
            m_MinDuration = 3600000.0f / ((voltage * maxAmp) * (1000.0f / tickByKW));

            HALIB_ADAPTER_DEBUG_MSG("ConstructorEND\n");
        }

        virtual ~HAAdapterDDS238()
        {
            suspend(true);

            m_Persistent.tag = 0;
            m_PersistenceFunction(m_Persistent);

            if (m_pDevice != NULL)
            {
                m_pDevice->removeComponent(m_pCumulaticComponent);
                m_pDevice->removeComponent(m_pInstantComponent);
            }
            delete m_pCumulaticComponent;
            delete m_pInstantComponent;
        }

        virtual void setDevice(HADevice *p_pDevice)
        {
            HALIB_ADAPTER_DEBUG_MSG("setDevice\n");
            if (m_pDevice != NULL)
            {
                HALIB_ADAPTER_DEBUG_MSG("remove from previous\n");
                m_pDevice->removeComponent(m_pCumulaticComponent);
                m_pDevice->removeComponent(m_pInstantComponent);
            }
            m_pDevice = p_pDevice;
            m_pDevice->addComponent(m_pCumulaticComponent);
            m_pDevice->addComponent(m_pInstantComponent);
            HALIB_ADAPTER_DEBUG_MSG("setDeviceEND\n");
        }

        virtual void restore(DDS238Data persistence)
        {
            HALIB_ADAPTER_DEBUG_MSG("restore\n");
            // Restore only if compliant or new value
            if ((persistence.tag == MAGICTAG) || (persistence.tag == DEFAULTTAG))
            {
                if (persistence.cumulative > 999999.9f || persistence.cumulative < 0.0f || isnan(persistence.cumulative))
                {
                    HALIB_ADAPTER_DEBUG_MSG("Corruption detected: %f. Reset to 0.\n", persistence.cumulative);
                    m_Persistent.cumulative = 0.0f;
                    m_Persistent.ticks = 0;
                }
                else
                {
                    m_Persistent.cumulative = persistence.cumulative;
                    m_Persistent.ticks = persistence.ticks;
                }
                // Send value to component
                m_pCumulaticComponent->setValue(m_Persistent.cumulative);
            }
            else
            {
                HALIB_ADAPTER_DEBUG_MSG("restored with default\n");
                m_Persistent.cumulative = 0.0f;
                m_Persistent.ticks = 0;
            }

#ifdef ESP8266
            if (m_rtcOffset >= 0)
            {
                DDS238Data rtcData;
                if (ESP.rtcUserMemoryRead(m_rtcOffset, (uint32_t *)&rtcData, sizeof(DDS238Data)))
                {
                    if (rtcData.tag == MAGICTAG)
                    {
                        if (!isnan(rtcData.cumulative) && rtcData.cumulative >= m_Persistent.cumulative)
                        {
                            HALIB_ADAPTER_DEBUG_MSG("Restored from RTC\n");
                            m_Persistent = rtcData;
                        }
                    }
                }
            }
#endif
            if (persistence.tag != MAGICTAG)
                m_PersistenceFunction(m_Persistent);

            HALIB_ADAPTER_DEBUG_MSG("restoreEND\n");
        }

        virtual void loop()
        {
            // Check if ticks happened since last loop
            if ((m_pulseCount > 0) && (m_Persistent.cumulative < 999999.9f))
            {
                int nbTick = 0;
                unsigned long tickTime = 0;

                // CRITICAL SECTION: Read and reset volatile variables
                noInterrupts();
                nbTick = m_pulseCount;
                m_pulseCount = 0;
                tickTime = m_lastPulseTimestamp; // Updated variable name
                interrupts();

                // Add tick to persistent counter
                m_Persistent.ticks += nbTick;

                // If number of ticks is bigger than a tenth of Kw
                if (m_Persistent.ticks > m_TickByTenthKW)
                {
                    m_Persistent.cumulative += 0.1;
                    m_Persistent.ticks -= m_TickByTenthKW;
                    m_pCumulaticComponent->setValue(m_Persistent.cumulative);
                }

                // Instant power management
                if (0 != m_LastTickTreatedTime) // Skip first tick
                {

                    unsigned long deltaT = tickTime - m_LastTickTreatedTime;
                    // overflow is automaticly managed because of unsigned according to Gemini
                    if (deltaT > 0)
                    {
                        m_InstantPower = m_AverageActive ? computeAverage(nbTick, deltaT) : computeInstant(nbTick, deltaT);
                        float rounded = (floorf(10 * m_InstantPower)) / 10;
                        m_pInstantComponent->setValue(rounded);
                    }
                    m_LastTickDeltaTime = deltaT;
                }

                m_LastTickTreatedTime = tickTime;
                // Store data at each tick
                m_PersistenceFunction(m_Persistent);
#ifdef ESP8266
                if (m_rtcOffset >= 0)
                {
                    ESP.rtcUserMemoryWrite(m_rtcOffset, (uint32_t *)&m_Persistent, sizeof(DDS238Data));
                }
#endif
            }
            else
            {
                // Auto decrement if no more instant power
                if (m_InstantPower != 0)
                {
                    // If no pulse for a long time (10x the instant factor duration), assume 0W
                    if ((millis() - m_LastTickTreatedTime) > (10 * m_InstantFactor))
                    {
                        m_InstantPower = 0;
                        m_AverageDataSet.nbTick = 0;
                        m_AverageDataSet.duration = 0;
                        m_AverageBuffer.clear();

                        m_pInstantComponent->setValue(0);
                    }
                }
            }
        }

        virtual void _setup()
        {
            pinMode(m_pinNumber, INPUT_PULLUP);
        }

        static void ISR_PREFIX isrTrampoline(void *arg)
        {
            HAAdapterDDS238 *instance = static_cast<HAAdapterDDS238 *>(arg);
            if (instance)
            {
                instance->onTick();
            }
        }
        virtual void suspend(boolean state)
        {
            if (state)
            {
                detachInterrupt(digitalPinToInterrupt(m_pinNumber));
            }
            else
            {
                attachInterruptArg(
                    digitalPinToInterrupt(m_pinNumber),
                    isrTrampoline,
                    this,
                    CHANGE);
            }
        }

        virtual int toJsonDebug(char *jsonBuffer, size_t length)
        {
            return snprintf_P(jsonBuffer, length,
                              PSTR("{\"type\":\"dds\",\"pin\":%d,\"pt\":%u,\"pc\":%.2f,\"lint\":%lu,\"pls\":%lu,\"lplsd\":%lu,\"ltt\":%lu,\"ltd\":%lu}"),
                              m_pinNumber,
                              m_Persistent.ticks,
                              m_Persistent.cumulative,
                              lastInterruptTime,
                              m_pulseCount,
                              lastpulseduration,
                              m_LastTickTreatedTime,
                              m_LastTickDeltaTime);
        };
        virtual String stringProcessor(const String &variable)
        {
            if (variable == "PIN")
            {
                return String(m_pinNumber); // Instant value for page load
            }
            if (variable == "LASTTT")
            {
                return String(lastInterruptTime); // Instant value for page load
            }
            if (variable == "PULSE")
            {
                return String(m_pulseCount); // Instant value for page load
            }
            if (variable == "LASTPLSD")
            {
                return String(lastpulseduration); // Instant value for page load
            }
            if (variable == "LASTDELTATT")
            {
                return String(m_LastTickDeltaTime); // Instant value for page load
            }
            if (variable == "LASTTREATT")
            {
                return String(m_LastTickTreatedTime); // Instant value for page load
            }
            return "";
        }

    protected:
        struct InstantMeasure
        {
            unsigned long duration = 0;
            int nbTick = 0;
        };

        float computeInstant(int p_nbTick, unsigned long p_duration)
        {
            if (p_duration == 0)
                return 0;
            return (float)p_nbTick * m_InstantFactor / (float)p_duration;
        }

        float computeAverage(int p_nbTick, unsigned long p_duration)
        {
            InstantMeasure newMeasure;
            newMeasure.duration = p_duration;
            newMeasure.nbTick = p_nbTick;

            const InstantMeasure *pRemovedMeasure = m_AverageBuffer.push(newMeasure);

            if (NULL != pRemovedMeasure)
            {
                m_AverageDataSet.nbTick -= pRemovedMeasure->nbTick;
                m_AverageDataSet.duration -= pRemovedMeasure->duration;
            }
            m_AverageDataSet.nbTick += newMeasure.nbTick;
            m_AverageDataSet.duration += newMeasure.duration;

            if (m_AverageDataSet.duration == 0)
                return 0;
            return (float)m_AverageDataSet.nbTick * m_InstantFactor / (float)m_AverageDataSet.duration;
        }

        ushort m_TickByTenthKW;
        unsigned int m_MinDuration;
        boolean m_AverageActive = false;

        InstantMeasure m_AverageDataSet;
        BufferFIFO<InstantMeasure> m_AverageBuffer;

        int m_rtcOffset;
        HA_DDS238_PERSISTENT_FUNCTION m_PersistenceFunction;
        HADevice *m_pDevice;
        HAComponentSensor *m_pCumulaticComponent;
        HAComponentSensor *m_pInstantComponent;
        DDS238Data m_Persistent;

        // ---------------------------------------------------------
        // Volatile Variables (Modified by ISR)
        // ---------------------------------------------------------
        volatile unsigned long m_pulseStartTime;     // Start of the LOW pulse
        volatile unsigned long m_lastPulseTimestamp; // Time of the last valid pulse
        volatile unsigned long m_pulseCount;         // Number of pulses accumulated

        float m_InstantFactor;
        unsigned long m_LastTickTreatedTime = 0;
        unsigned long m_LastTickDeltaTime = 0;
        float m_InstantPower = 0;
        unsigned long lastInterruptTime = 0;
        unsigned long lastpulseduration = 0;

        /**
         * Interrupt Service Routine for S0 Pulse Counting.
         */
        void ISR_PREFIX onTick()
        {
            unsigned long now = millis();

            // 1. "Software Debounce" / Rate Limiting (10ms)
            if ((now - lastInterruptTime) < 10)
            {
                return;
            }
            lastInterruptTime = now;

            // 2. Read GPIO State
            bool pinState = digitalRead(m_pinNumber);

            if (pinState == LOW)
            {
                // FALLING EDGE (Start of Active-Low Pulse)
                m_pulseStartTime = now;
            }
            else
            {
                // RISING EDGE (End of Active-Low Pulse)
                if (m_pulseStartTime != 0)
                {
                    lastpulseduration = now - m_pulseStartTime;

                    // 3. Validation (EN 62053-31 Standard: >30ms)
                    if (lastpulseduration >= 30)
                    {
                        m_pulseCount++;
                        m_lastPulseTimestamp = now;
                    }
                    m_pulseStartTime = 0;
                }
            }
        }
    };
} // namespace HALIB_NAMESPACE