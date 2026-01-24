// CHANGEMENT ICI : .h -> .hpp
#include "HAComponent.h"
#include "../../tools/HAUtils.h"
#include "HAComponentPropertyConstante.hpp"
#include "HAComponentPropertyAction.hpp"
#include <pgmspace.h>
#include <stdio.h>

// CHANGEMENT ICI : .h -> .hpp
#include "../node/HANode.h"

namespace HALIB_NAMESPACE
{
    const char CONFIG_TOPIC_TEMPLATE[] PROGMEM = "%s/%s/%s/%s-%08X/config";
    const char ROOT_TOPIC_TEMPLATE[] PROGMEM = "%s/%s/%s-%s-%08X";

    HAComponent::HAComponent(const char *pName, ComponentType pComponentType)
    {
        m_pNode = NULL;
        isFirstHAConnection = true;

        addProperty(PROP_NAME, pName);
        m_ComponentType = pComponentType;
        m_ComponentId = HAUtils::generateId(pName, pComponentType);

        char *out = (char *)malloc(9 * sizeof(char));
        memset(out, 0, 9 * sizeof(char));
        sprintf(out, "%08X", m_ComponentId);
        addProperty(PROP_UNIQUE_ID, out);
        free(out);
    }

    HAComponent::~HAComponent()
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
    }

    const char *HAComponent::getName()
    {
        return getPropertyValue(PROP_NAME);
    }

    uint32_t HAComponent::getId()
    {
        return m_ComponentId;
    }

    void HAComponent::setNode(IHANode *pNnode)
    {
        if (NULL != pNnode)
        {
            m_pNode = pNnode;
            char *rootTopic = buildRootTopic(m_pNode->getName());
            addProperty(PROP_ROOT_TOPIC, rootTopic);
            free(rootTopic);

            for (int index = 0; index < mActions.getSize(); index++)
            {
                HAComponentProperty *pComponentProperty = (HAComponentProperty *)mActions.get(index);

                int pathLen = getTopicPath(pComponentProperty->getValue());
                char topic[pathLen + 1];
                getTopicPath(pComponentProperty->getValue(), topic);

                m_pNode->registerToHA(topic);
            }
        }
        else
        {
            m_pNode = NULL;
        }
    }

    char *HAComponent::buildDiscoveryTopic(const char *pDiscoveryPrefix, const char *pNodeId)
    {
        if (pNodeId == NULL)
        {
            pNodeId = m_pNode->getName();
        }

        PGM_P typePtr = (PGM_P)pgm_read_ptr(&componentTypeTag[m_ComponentType]);

        int topicLength = 17 + strlen(pDiscoveryPrefix) + strlen_P(typePtr) + strlen(pNodeId) + strlen(getPropertyValue(PROP_NAME)) + 8;

        char *topic = (char *)malloc(topicLength + 1);
        topic[topicLength] = 0;

        sprintf_P(topic, CONFIG_TOPIC_TEMPLATE, pDiscoveryPrefix, typePtr, pNodeId, getPropertyValue(PROP_NAME), m_ComponentId);

        return topic;
    }

    char *HAComponent::buildDiscoveryMessage()
    {
        size_t size = 2;

        for (int index = 0; index < mProperties.getSize(); index++)
        {
            HAComponentProperty *pComponentProperty = mProperties.get(index);
            size += pComponentProperty->getJson();
            size++;
        }
        for (int index = 0; index < mActions.getSize(); index++)
        {
            HAComponentProperty *pComponentProperty = mActions.get(index);
            size += pComponentProperty->getJson();
            size++;
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

    bool HAComponent::onHAMessage(const char *topic, const byte *p_pPayload, const unsigned int length)
    {
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

                if (strncmp(componentRootTopic, topic, strlen(componentRootTopic)) == 0)
                {
                    if (strncmp(topic + strlen(componentRootTopic), value + 1, strlen(value) - 1) == 0)
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

    void HAComponent::_onHAConnect()
    {
        if (isFirstHAConnection)
        {
            m_pNode->postDiscoveryMessage(this);
            isFirstHAConnection = false;
        }
        onHAConnect();
    }

    void HAComponent::onHAConnect()
    {
    }

    bool HAComponent::operator==(IHAComponent &other)
    {
        return other.getId() == getId();
    }

    bool HAComponent::operator!=(IHAComponent &other)
    {
        return other.getId() != getId();
    }

    void HAComponent::addAction(HAComponentProperty *p_pProperty)
    {
        mActions.append(p_pProperty);

        if (NULL != m_pNode)
        {
            int pathLen = getTopicPath(p_pProperty->getValue());
            char topic[pathLen + 1];
            getTopicPath(p_pProperty->getValue(), topic);

            m_pNode->registerToHA(topic);
        }
    }

    HAComponentProperty *HAComponent::addProperty(HAComponentPropertyKey pName, const char *pValue)
    {
        HAComponentProperty *pProperty = new HAComponentProperty(pName, pValue);
        mProperties.append(pProperty);
        return pProperty;
    }

    char *HAComponent::buildRootTopic(const char *pNodeId)
    {
        PGM_P typePtr = (PGM_P)pgm_read_ptr(&componentTypeTag[m_ComponentType]);

        int topicLength = 4 + 13 + strlen_P(typePtr) + strlen(pNodeId) + strlen(getPropertyValue(PROP_NAME)) + 8;

        char *topic = (char *)malloc(topicLength + 1);
        topic[topicLength] = 0;

        sprintf_P(topic, ROOT_TOPIC_TEMPLATE, "homeassistant", typePtr, pNodeId, getPropertyValue(PROP_NAME), m_ComponentId);

        return topic;
    }

    int HAComponent::getTopicPath(const char *topic, char *out)
    {
        if (NULL == topic)
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

    const char *HAComponent::getRootTopic()
    {
        return getPropertyValue(PROP_ROOT_TOPIC);
    }

    int HAComponent::getTopicPath(HAComponentPropertyKey pName, char *out)
    {
        const char *topic = getPropertyValue(pName);
        return getTopicPath(topic, out);
    }

    const char *HAComponent::getPropertyValue(HAComponentPropertyKey name, boolean fallbackDefault)
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
                    return addProperty(name)->getValue();
                }
                else
                {
                    return NULL;
                }
            }
        }
    }

    void HAComponent::_executeAction(HAComponentProperty *p_pAction, const byte *payload, const unsigned int length)
    {
    }

} // namespace HALIB_NAMESPACE