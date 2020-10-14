#pragma once

#include <Arduino.h>
#include "../../tools/LinkedList.hpp"
#include "../../tools/HAUtils.hpp"
#include "../HASubscribCmd.hpp"
#include "../HAMessage.hpp"
#include "../IHANode.h"
#include "../IHAComponent.h"
#include "../components/HAComponentType.hpp"
//#include "../components/HAComponentProperty.hpp"
//#include "../components/HAComponentPropertyConstante.hpp"

namespace HALIB_NAMESPACE
{
    const char AVAILABILITY_TOPIC_TEMPLATE[] PROGMEM = "homeassistant/%s-%08X/avty";
    class HANode : public IHANode
    {
        friend class HAComponent;
    public:
        HANode(const char *pName)
        {
            mName = strdup(pName);
            mId = HAUtils::generateId(pName, DEVICE);

            /* AVAILABILILY */
            char *out = (char *)calloc(strlen(mName) + 20 + 8 + 1, sizeof(char));
            sprintf_P(out, AVAILABILITY_TOPIC_TEMPLATE, mName, mId);
            mProperties.append(new HAComponentProperty(PROP_AVAILABILITY_TOPIC, out));
            mProperties.append(new HAComponentProperty(PROP_PAYLOAD_AVAILABLE, "online"));
            mProperties.append(new HAComponentProperty(PROP_PAYLOAD_NOT_AVAILABLE, "offline"));
            free(out);
        }
        virtual ~HANode()
        {
            if (NULL != mName)
            {
                free(mName);
            }

            HAAction* pAction = mOutboxAction.shift();
            while (NULL != pAction)
            {
                delete (pAction);
                pAction = mOutboxAction.shift();
            }

             HAUtils::deleteProperties(mProperties);
        }
        void setDeviceInfo(const char *pManuf, const char *pModel, const char *pRelease)
        {
            LinkedList<HAComponentProperty *> lProperties;

            if (NULL != pManuf)
            {
                lProperties.append(new HAComponentProperty(PROP_DEVICE_MANUFACTURER, pManuf));
            }
            if (NULL != pModel)
            {
                lProperties.append(new HAComponentProperty(PROP_DEVICE_MODEL, pModel));
            }
            if (NULL != pRelease)
            {
                lProperties.append(new HAComponentProperty(PROP_DEVICE_RELEASE, pRelease));
            }

            lProperties.append(new HAComponentProperty(PROP_NAME, mName));



            char *out = (char *)malloc(9 * sizeof(char));
            memset(out, 0, 9 * sizeof(char));
            sprintf(out, "%08X", mId);
            lProperties.append(new HAComponentProperty(PROP_DEVICE_IDENTIFIER, out));
            free(out);


            out = HAUtils::propertyListToJson(lProperties);
            HAUtils::deleteProperties(lProperties);

            mProperties.append(new HAComponentProperty(PROP_DEVICE, out));
            free(out);
        }

        //IHANode
        const char *getName()
        {
            return mName;
        }
        uint32_t getId()
        {
            return mId;
        };
        const char* getProperty(HAComponentPropertyKey name){
            return HAUtils::getProperty(mProperties, name);
        }
        void addComponent(IHAComponent *p_pComponent)
        {
            if(NULL != p_pComponent)
            {
                p_pComponent->setNode(this);
                mComponents.append(p_pComponent);
            }
        }

        void postAutoDiscovery()
        {
            DEBUG_PRINTLN("===>postAutoDiscovery");

            char *tempDeviceInfo = HAUtils::propertyListToJson(mProperties);
            size_t deviceInfoLength = strlen(tempDeviceInfo) - 2; // -2 remove {}

            // Treat all component
            for (int index = 0; index < mComponents.getSize(); index++)
            {
                IHAComponent *pComponent = (IHAComponent *)mComponents.get(index);
                char *topic = pComponent->buildDiscoveryTopic("homeassistant", mName);
                char *componentDiscoveryMessage = pComponent->buildDiscoveryMessage();

                char *discoveryMessage = componentDiscoveryMessage;
                
                // if device info present patch component discovery message
                if (deviceInfoLength != 0)
                {
                    size_t discoveryMessagesLength = strlen(componentDiscoveryMessage);
                    discoveryMessage = (char *)malloc(discoveryMessagesLength + 1 + deviceInfoLength + 1);

                    strcpy(discoveryMessage, componentDiscoveryMessage);
                    strcpy(discoveryMessage + discoveryMessagesLength - 1, ",");
                    strncpy(discoveryMessage + discoveryMessagesLength - 1 + 1, tempDeviceInfo + 1, deviceInfoLength); //+1 to skip first {
                    strcpy(discoveryMessage + discoveryMessagesLength - 1 + 1 + deviceInfoLength, "}");

                    free(componentDiscoveryMessage);
                }
                //DEBUG_PRINTLN(topic);
                //DEBUG_PRINTLN(discoveryMessage);
                postMessage(new HAMessage(topic, discoveryMessage, true));
            }
            free(tempDeviceInfo);
        };

        void postDiscoveryMessage(IHAComponent *pComponent){
            DEBUG_PRINTLN("===>postDiscoveryMessage");
            char *tempDeviceInfo = HAUtils::propertyListToJson(mProperties);
            size_t deviceInfoLength = strlen(tempDeviceInfo) - 2;
            char *topic = pComponent->buildDiscoveryTopic("homeassistant", mName);
            char *componentDiscoveryMessage = pComponent->buildDiscoveryMessage();

            char *discoveryMessage = componentDiscoveryMessage;
            
            // if device info present patch component discovery message
            if (deviceInfoLength != 0)
            {
                size_t discoveryMessagesLength = strlen(componentDiscoveryMessage);
                discoveryMessage = (char *)malloc(discoveryMessagesLength + 1 + deviceInfoLength + 1);

                strcpy(discoveryMessage, componentDiscoveryMessage);
                strcpy(discoveryMessage + discoveryMessagesLength - 1, ",");
                strncpy(discoveryMessage + discoveryMessagesLength - 1 + 1, tempDeviceInfo + 1, deviceInfoLength); //+1 to skip first {
                strcpy(discoveryMessage + discoveryMessagesLength - 1 + 1 + deviceInfoLength, "}");

                free(componentDiscoveryMessage);
            }
            
            // DEBUG_PRINT("topic: ");
            // DEBUG_PRINTLN(topic);
            // DEBUG_PRINT("discoveryMessage: ");
            // DEBUG_PRINTLN(discoveryMessage);
            postMessage(new HAMessage(topic, discoveryMessage, true));
            free(topic);
            free(discoveryMessage);
            free(tempDeviceInfo);
        }

        bool onHAMessage(const char *topic, const byte* p_pPayload, const unsigned int length)
        {
            
            for (int index = 0; index < mComponents.getSize(); index++)
            {
                // get component at index
                IHAComponent *pComponent = (IHAComponent *)mComponents.get(index);

                if (pComponent->onHAMessage(topic, p_pPayload, length))
                {
                    return true;
                }
            }
            return false;
        }

        void postMessage(HAMessage *pMessage)
        {
            mOutboxAction.append(pMessage);
        }


        void onHAConnect()
        {
            for (int index = 0; index < mComponents.getSize(); index++)
            {
                // get component at index
                IHAComponent *pComponent = (IHAComponent *)mComponents.get(index);
                pComponent->_onHAConnect();
            }
        }
        int getDiscoveryMessages(HAMessage *pMessages)
        {
            if (NULL != pMessages)
            {
                for (int index = 0; index < mComponents.getSize(); index++)
                {
                    IHAComponent *pComponent = (IHAComponent *)mComponents.get(index);
                    char *topic = pComponent->buildDiscoveryTopic("homeassistant", mName);
                    char *discoveryMessage = pComponent->buildDiscoveryMessage();

                    pMessages[index].setTopic(topic);
                    pMessages[index].setMessage(discoveryMessage);
                }
            }
            return mComponents.getSize();
            ;
        }

        HAAction *pickupAction()
        {
            // DEBUG_PRINT("pickupAction ");
            // DEBUG_PRINTLN(mOutboxAction.getSize());
            return mOutboxAction.shift();
        }
        int actionsSize(){
            return mOutboxAction.getSize();
        }

        void retryAction(HAAction* p_pAction){
            mOutboxAction.append(p_pAction);
        }

    private:
#ifdef UNIT_TEST
    public:
#endif

        void postMessage(const char *pTopic, const char *pMessage, boolean retain)
        {
            postMessage(new HAMessage(pTopic, pMessage, retain));
        }
        void registerToHA(const char *topic, boolean subscription = true)
        {
            DEBUG_PRINTLN("===>postSupscribe");
            mOutboxAction.append(new HASubscribCmd(topic, subscription));
        }

    private:
        char *mName;
        uint32_t mId;
        LinkedList<IHAComponent *> mComponents;
        LinkedList<HAAction *> mOutboxAction;
        LinkedList<HAComponentProperty *> mProperties;
    };
} // namespace HALIB_NAMESPACE