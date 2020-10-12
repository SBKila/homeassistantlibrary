#pragma once

#include <Arduino.h>
#include "../../tools/LinkedList.hpp"
#include "../../tools/HAUtils.hpp"
#include "../IHAComponent.h"
#include "HAComponentType.hpp"
#include "HAComponentProperty.hpp"
#include "HAComponentPropertyAction.hpp"
#include "HAComponentPropertyConstante.hpp"
#include "HADeviceClass.hpp"
#include "../../arduino/HACallbackFunction.h"

namespace HALIB_NAMESPACE
{
    const char CONFIG_TOPIC_TEMPLATE[] PROGMEM = "%s/%s/%s/%s-%08X/config";
    const char ROOT_TOPIC_TEMPLATE[] PROGMEM = "%s/%s/%s-%s-%08X";
    
    class HAComponent : public IHAComponent
    {
    public:
        HAComponent(const char *pName, ComponentType pComponentType)
        {
            m_pNode = NULL;

            // store device name
            addProperty(PROP_NAME, pName);

            // store Component Type
            m_ComponentType = pComponentType;

            // generate devideID
            m_ComponentId = HAUtils::generateId(pName, pComponentType);

            char *out = (char *)malloc(9 * sizeof(char));
            memset(out, 0, 9 * sizeof(char));
            sprintf(out, "%08X", m_ComponentId);
            addProperty(PROP_UNIQUE_ID, out);
            free(out);
        }
        virtual ~HAComponent()
        {
            HAComponentProperty *pProperty = mProperties.shift();
            while (NULL != pProperty)
            {
                delete (pProperty);
                pProperty = (HAComponentProperty *)mProperties.shift();
            }
            pProperty = mActions.shift();
            while (NULL != pProperty)
            {
                delete (pProperty);
                pProperty = (HAComponentProperty *)mActions.shift();
            }

            // free memory allocated in constructor
        }

        const char *getName()
        {
            return getPropertyValue(PROP_NAME);
        };
        virtual uint32_t getId()
        {
            return m_ComponentId;
        };

        virtual void setNode(IHANode *pNnode)
        {
            m_pNode = pNnode;
            char *rootTopic = buildRootTopic(m_pNode->getName());
            addProperty(PROP_ROOT_TOPIC, rootTopic);

            for (int index = 0; index < mActions.getSize(); index++)
            {
                HAComponentProperty *pComponentProperty = (HAComponentProperty *)mActions.get(index);
                char topic[getTopicPath(pComponentProperty->getValue()) + 1];
                getTopicPath(pComponentProperty->getValue(), topic);

                m_pNode->registerToHA(topic);
            }

            delete rootTopic;
        }
        virtual char *buildDiscoveryTopic(const char *pDiscoveryPrefix, const char *pNodeId = NULL)
        {
            if (pNodeId == NULL)
            {
                pNodeId = m_pNode->getName();
            }

            char *componentType = HAUtils::strdup_P(componentTypeTag[m_ComponentType]);

            int topicLength = 17 + strlen(pDiscoveryPrefix) + strlen(componentType) + strlen(pNodeId) + strlen(getPropertyValue(PROP_NAME)) + 8;

            char *topic = (char *)malloc(topicLength + 1);
            // zero ending
            topic[topicLength] = 0;

            // <discovery_prefix>/<component>/[<node_id>/]<object_id>/config
            sprintf_P(topic, CONFIG_TOPIC_TEMPLATE, pDiscoveryPrefix, componentType, pNodeId, getPropertyValue(PROP_NAME), m_ComponentId);

            free(componentType);
            return topic;
        }
        virtual char *buildDiscoveryMessage()
        {
            size_t size = 2;

            for (int index = 0; index < mProperties.getSize(); index++)
            {
                HAComponentProperty *pComponentProperty = mProperties.get(index);
                size += pComponentProperty->getJson();
                size++;
                //size += (index == 0) ? 0 : 1;
            }
            for (int index = 0; index < mActions.getSize(); index++)
            {
                HAComponentProperty *pComponentProperty = mActions.get(index);
                size += pComponentProperty->getJson();
                size++;
                //size += (index == 0) ? 0 : 1;
            }

            char *discoveryString = (char *)calloc(size + 1, sizeof(char));
            char *currentPosition = discoveryString;
            *currentPosition = '{';
            currentPosition++;

            for (int index = 0; index < mProperties.getSize(); index++)
            {
                HAComponentProperty *pComponentProperty = (HAComponentProperty *)mProperties.get(index);
                if (currentPosition != discoveryString + 1)
                {
                    *currentPosition = ',';
                    currentPosition++;
                }
                currentPosition += pComponentProperty->getJson(&currentPosition);
            }
            for (int index = 0; index < mActions.getSize(); index++)
            {
                HAComponentProperty *pComponentProperty = (HAComponentProperty *)mActions.get(index);
                if (currentPosition != discoveryString + 1)
                {
                    *currentPosition = ',';
                    currentPosition++;
                }
                currentPosition += pComponentProperty->getJson(&currentPosition);
            }

            *currentPosition = '}';
            return discoveryString;
        }

        virtual bool onHAMessage(const char *topic, const byte *p_pPayload, const unsigned int length)
        {
            //DEBUG_PRINT("onHaMessage : ");DEBUG_PRINTLN(topic);
            bool match = false;
            int index = 0;
            HAComponentProperty *pComponentProperty = NULL;
            
            while (index < mActions.getSize())
            {
                pComponentProperty = (HAComponentProperty *)mActions.get(index);
                const char *value = pComponentProperty->getValue();
                if ('~' == *value)
                {
                    const char *componentRootTopic = getPropertyValue(PROP_ROOT_TOPIC);

                    if
                     (strncmp(componentRootTopic, topic, strlen(componentRootTopic)) == 0)
                    {
                        if (strncmp(topic + strlen(componentRootTopic), value + 1, strlen(value - 1)) == 0)
                        {
                            match = true;
                            break;
                        }
                    }
                }
                else
                {
                    if ((strlen(topic) == strlen(value)) && (0 == strcmp(topic, value)))
                    {
                        match = true;
                        break;
                    }
                }

                index++;
            }
            if (match)
            {
                _executeAction(pComponentProperty, p_pPayload, length);
            }

            return match;
        }
        virtual void onHAConnect(){};
        virtual bool operator==(IHAComponent &other)
        {
            return other.getId() == getId();
        }
        virtual bool operator!=(IHAComponent &other)
        {
            return other.getId() != getId();
        }

        void addAction(HAComponentProperty *p_pProperty)
        {
            mActions.append(p_pProperty);

            if (NULL != m_pNode) // add action on the fly
            {
                char topic[getTopicPath(p_pProperty->getValue()) + 1];
                getTopicPath(p_pProperty->getValue(), topic);
                m_pNode->registerToHA(topic);
            }
        }
        HAComponentProperty *addProperty(HAComponentPropertyKey pName, const char *pValue = NULL)
        {
            HAComponentProperty *pProperty = new HAComponentProperty(pName, pValue);
            mProperties.append(pProperty);
            return pProperty;
        }

    private:
        /**
        * allocate a string containing the configTopic
        */
        ComponentType m_ComponentType;
        uint32_t m_ComponentId;
        LinkedList<HAComponentProperty *> mProperties;
        LinkedList<HAComponentProperty *> mActions;

#ifdef UNIT_TEST
    public:
#endif
        char *buildRootTopic(const char *pNodeId)
        {
            char *componentType = HAUtils::strdup_P(componentTypeTag[m_ComponentType]);

            int topicLength = 4 + 13 + strlen(componentType) + strlen(pNodeId) + strlen(getPropertyValue(PROP_NAME)) + 8;
            char *topic = (char *)malloc(topicLength + 1);
            // zero ending
            topic[topicLength] = 0;

            // homeassistant/<component>/>node_id>-<objectName>-<objectId>
            sprintf_P(topic, ROOT_TOPIC_TEMPLATE, "homeassistant", componentType, pNodeId, getPropertyValue(PROP_NAME), m_ComponentId);

            free(componentType);
            return topic;
        }

        int getTopicPath(const char *topic, char *out = NULL)
        {
            if(NULL==topic)
                return 0;
            
            const char *preTopic = NULL;

            size_t topicLength = strlen(topic);
            size_t preTopicLength = 0;

            boolean isCollapsedPath = (*topic == '~');

            if (isCollapsedPath)
            {
                topicLength--;
                preTopic = getPropertyValue(PROP_ROOT_TOPIC);
                if (NULL != preTopic)
                    preTopicLength = strlen(preTopic);
            }

            if (out != NULL)
            {
                if (NULL != preTopic)
                {
                    strcpy(out, preTopic);
                    strcpy(out + preTopicLength, topic + 1);
                }
                else
                {
                    strcpy(out, topic + (isCollapsedPath ? 1 : 0));
                }
            }

            return preTopicLength + topicLength;
        }
        const char *getRootTopic()
        {
            return getPropertyValue(PROP_ROOT_TOPIC);
        }

    protected:
#ifdef UNIT_TEST
    public:
#endif
        IHANode *m_pNode;
        int getTopicPath(HAComponentPropertyKey pName, char *out = NULL)
        {
            const char *topic = getPropertyValue(pName);
            return getTopicPath(topic, out);
        }

        const char *getPropertyValue(HAComponentPropertyKey name, boolean fallbackDefault = false)
        {
            HAComponentProperty propertyToFind(name, "");
            int propertyIndex = mActions.find(&propertyToFind, true);
            if (propertyIndex != -1)
            {
                return ((HAComponentProperty *)mActions.get(propertyIndex))->getValue();
            }
            else
            {
                propertyIndex = mProperties.find(&propertyToFind, true);
                if (propertyIndex != -1)
                {
                    return ((HAComponentProperty *)mProperties.get(propertyIndex))->getValue();
                }
                else
                {
                    if (fallbackDefault)
                    {
                        HAComponentProperty *pProperty = new HAComponentProperty(name, HAUtils::strdup_P(PropertyDefaultValue[name]));
                        addProperty(name);
                        return pProperty->getValue();
                    }
                    else
                    {
                        return NULL;
                    }
                }
            }
        }

        virtual void _executeAction(HAComponentProperty *p_pAction, const byte *payload, const unsigned int length){};
    };
} // namespace HALIB_NAMESPACE