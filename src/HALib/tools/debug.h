#include <Arduino.h>

#ifdef DEBUG_HALIB_DEVICE
#define HALIB_DEVICE_DEBUG_MSG(...) DEBUG_MSG("MAIN", __VA_ARGS__)
#define HALIB_ADAPTER_DEBUG_MSG(...) DEBUG_MSG("[HALIB] Adapter: ", __VA_ARGS__)
#define HALIB_COMPONENT_DEBUG_MSG(...) DEBUG_MSG("[HALIB] Component: ", __VA_ARGS__)
#define HALIB_NODE_DEBUG_MSG(...) DEBUG_MSG("[HALIB] Node: ", __VA_ARGS__)
#else
#define HALIB_DEVICE_DEBUG_MSG(...)
#define HALIB_ADAPTER_DEBUG_MSG(...)
#define HALIB_COMPONENT_DEBUG_MSG(...)
#define HALIB_NODE_DEBUG_MSG(...)
#endif