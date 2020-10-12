#ifndef HACALLBACKFUNCTION_H
#define HACALLBACKFUNCTION_H
#include <Arduino.h>

typedef void (*HACallBackFunction)();
typedef void (*HALigthBasicCallBackFunction)(boolean);
#define HA_BASICLIGHT_CALLBACK std::function<void(boolean powerOn)>
#define HA_BASICLIGHT_CALLBACK_SIGNATURE HA_BASICLIGHT_CALLBACK callback

#endif