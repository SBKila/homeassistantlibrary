#pragma once

#include "../device/HADevice.hpp"
#include "HAAdapter.hpp"

namespace HALIB_NAMESPACE
{
    class IHAAdapter
    {
    public:
        virtual const char *getName() const =0;
        virtual void setup()=0;
        virtual void setDevice(HADevice *) =0;
        virtual void suspend(boolean state) =0;
        virtual void loop() =0;
        //virtual JsonObject toJson() =0;

        virtual bool operator!=(const IHAAdapter &other) =0;
        virtual bool operator==(const IHAAdapter &other) =0;
   
    };
} // namespace HALIB_NAMESPACE