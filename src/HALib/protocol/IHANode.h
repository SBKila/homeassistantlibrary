#ifndef IHANODE_H
#define IHANODE_H
#include <Arduino.h>
namespace HALIB_NAMESPACE
{
    class IHAComponent;
    class IHANode
    {
    public:
        virtual const char *getName() =0 ;
        virtual uint32_t getId() =0;
        
        
        virtual void addComponent(IHAComponent *p_pComponent) =0;
        
        
        virtual boolean postMessage(const char *pTopic, const char *pMessage, boolean retain) =0 ;
        virtual void registerToHA(const char *topic, boolean subscription = true) =0 ;
        virtual bool onHAMessage(const char *topic, const byte *payload, const unsigned int length) =0;
    };

} // namespace HALIB_NAMESPACE
#endif