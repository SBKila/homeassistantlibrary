// HALibrary
// Copyright Stephane Blanc 2020
// MIT License
#pragma once
#include "HALib/arduino/device/HADevice.h"
#include "HALib/arduino/adapters/HAAdapterPushBtn.hpp"
#include "HALib/arduino/adapters/HAAdapterDDS238.hpp"
#include "HALib/arduino/adapters/HAAdapterLED.hpp"
#include "HALib/arduino/adapters/HAAdapter.h"
#include "HALib/protocol/components/HAComponentProperty.h"
#include "HALib/protocol/components/HAComponentLigthBasic.hpp"
#include "HALib/protocol/components/HAComponentBinarySensor.hpp"
#include "HALib/protocol/components/HAComponentDeviceTrigger.h"
#include "HALib/protocol/components/HAComponentSensor.hpp"

namespace HALib
{
    using HALIB_NAMESPACE::DDS238Data;
    using HALIB_NAMESPACE::HAAdapterDDS238;
    using HALIB_NAMESPACE::HAAdapterLed;
    using HALIB_NAMESPACE::HAAdapterPushBtn;
    using HALIB_NAMESPACE::HAComponentBinarySensor;
    using HALIB_NAMESPACE::HAComponentDeviceTrigger;
    using HALIB_NAMESPACE::HAComponentLigthBasic;
    using HALIB_NAMESPACE::HAComponentProperty;
    using HALIB_NAMESPACE::HAComponentPropertyKey;
    using HALIB_NAMESPACE::HAComponentSensor;
    using HALIB_NAMESPACE::HADevice;
}