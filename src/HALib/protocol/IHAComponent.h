#ifndef IHACOMPONENT_H
#define IHACOMPONENT_H
#include <Arduino.h>
#include "./IHANode.h"
namespace HALIB_NAMESPACE
{
    class IHANode;
    class IHAComponent
    {
    public:
        virtual ~IHAComponent() {}
        virtual uint32_t getId() = 0;
        virtual void setNode(IHANode *pNode) = 0;
        virtual char *buildDiscoveryTopic(const char *pDiscoveryPrefix, const char *pNodeId = NULL) = 0;
        virtual char *buildDiscoveryMessage() = 0;
        virtual bool onHAMessage(const char *topic, const byte *payload, const unsigned int length) = 0;
        virtual void _onHAConnect() = 0;
        virtual void onHAConnect() = 0;
        virtual bool operator==(IHAComponent &other) = 0;
        virtual bool operator!=(IHAComponent &other) = 0;
    };
} // namespace HALIB_NAMESPACE
#endif