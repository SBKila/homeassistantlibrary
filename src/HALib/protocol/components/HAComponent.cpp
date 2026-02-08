#include "HAComponent.h"
#include "../../tools/HAUtils.h"
#include "HAComponentPropertyConstante.hpp"
#include "HAComponentPropertyAction.hpp"
#include <pgmspace.h>
#include <stdio.h>
#include "../node/HANode.h"

namespace HALIB_NAMESPACE
{
    const char CONFIG_TOPIC_TEMPLATE[] PROGMEM = "%s/%s/%s/%s-%08X/config";
    const char ROOT_TOPIC_TEMPLATE[] PROGMEM = "%s/%s/%s-%s-%08X";

    HAComponent::HAComponent(const char *pName, ComponentType pComponentType)
    {
        HALIB_COMPONENT_DEBUG_MSG("Constructor %s %s\n", __func__, pName);
        m_pNode = NULL;
        isFirstHAConnection = true;

        addProperty(PROP_NAME, pName);
        m_ComponentType = pComponentType;
        m_ComponentId = HAUtils::generateId(pName, pComponentType);

        // --- OPTIMIZATION: Stack Allocation ---
        // Previously used malloc(9)/free() which caused heap fragmentation.
        // Using a fixed size char array on stack avoids heap usage completely here.
        char out[9];
        memset(out, 0, 9);
        sprintf(out, "%08X", m_ComponentId);

        // addProperty will handle the allocation/copy internally if needed.
        addProperty(PROP_UNIQUE_ID, out);

        HALIB_COMPONENT_DEBUG_MSG("ConstructorEND\n");
    }

    HAComponent::~HAComponent()
    {
        HALIB_COMPONENT_DEBUG_MSG("Destructor\n");

        // Cleanup Properties List
        HAComponentProperty *pProperty = mProperties.shift();
        while (NULL != pProperty)
        {
            delete (pProperty); // Triggers HAComponentProperty destructor (frees RAM if needed)
            pProperty = (HAComponentProperty *)mProperties.shift();
        }

        // Cleanup Actions List
        pProperty = mActions.shift();
        while (NULL != pProperty)
        {
            delete (pProperty);
            pProperty = (HAComponentProperty *)mActions.shift();
        }
        HALIB_COMPONENT_DEBUG_MSG("DestructorEND\n");
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
        HALIB_COMPONENT_DEBUG_MSG("setNode\n");
        if (NULL != pNnode)
        {
            m_pNode = pNnode;

            // Note: buildRootTopic allocates memory (malloc).
            char *rootTopic = buildRootTopic(m_pNode->getName());

            // addProperty copies the value because rootTopic is in RAM.
            addProperty(PROP_ROOT_TOPIC, rootTopic);

            // We free the temporary buffer.
            // Ideally, we could transfer ownership to avoid double alloc,
            // but the current architecture requires this copy-safety.
            free(rootTopic);

            for (int index = 0; index < mActions.getSize(); index++)
            {
                HAComponentProperty *pComponentProperty = (HAComponentProperty *)mActions.get(index);

                // Using VLA (Variable Length Array) on stack or simple calculation
                int pathLen = getTopicPath(pComponentProperty->getValue());

                // Warning: larger paths might overflow stack, but topics are generally short.
                char topic[pathLen + 1];
                getTopicPath(pComponentProperty->getValue(), topic);

                m_pNode->registerToHA(topic);
            }
        }
        else
        {
            m_pNode = NULL;
        }
        HALIB_COMPONENT_DEBUG_MSG("setNodeEND\n");
    }

    char *HAComponent::buildDiscoveryTopic(const char *pDiscoveryPrefix, const char *pNodeId)
    {
        HALIB_COMPONENT_DEBUG_MSG("buildDiscoveryTopic\n");
        if (pNodeId == NULL)
        {
            pNodeId = m_pNode->getName();
        }

        PGM_P typePtr = (PGM_P)pgm_read_ptr(&componentTypeTag[m_ComponentType]);

        int topicLength = 17 + strlen(pDiscoveryPrefix) + strlen_P(typePtr) + strlen(pNodeId) + strlen(getPropertyValue(PROP_NAME)) + 8;

        char *topic = (char *)malloc(topicLength + 1);
        if (topic != NULL)
        {
            topic[topicLength] = 0;
            sprintf_P(topic, CONFIG_TOPIC_TEMPLATE, pDiscoveryPrefix, typePtr, pNodeId, getPropertyValue(PROP_NAME), m_ComponentId);
        }

        HALIB_COMPONENT_DEBUG_MSG("buildDiscoveryTopicEND\n");
        return topic;
    }

    char *HAComponent::buildDiscoveryMessage()
    {
        HALIB_COMPONENT_DEBUG_MSG("buildDiscoveryMessage\n");
        size_t size = 2; // {}

        // 1. Calculate total size
        for (int index = 0; index < mProperties.getSize(); index++)
        {
            HAComponentProperty *pComponentProperty = mProperties.get(index);
            size += pComponentProperty->getJson();
            size++; // comma
        }
        for (int index = 0; index < mActions.getSize(); index++)
        {
            HAComponentProperty *pComponentProperty = mActions.get(index);
            size += pComponentProperty->getJson();
            size++; // comma
        }

        // 2. Allocate buffer
        char *discoveryString = (char *)calloc(size + 1, sizeof(char));
        if (discoveryString == NULL)
            return NULL; // Safety check

        char *currentPosition = discoveryString;
        *currentPosition = '{';
        currentPosition++;

        // 3. Fill buffer
        for (int index = 0; index < mProperties.getSize(); index++)
        {
            HAComponentProperty *pComponentProperty = (HAComponentProperty *)mProperties.get(index);
            if (currentPosition != discoveryString + 1)
            {
                *currentPosition = ',';
                currentPosition++;
            }
            // Pass pointer to currentPosition to append efficiently
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
        HALIB_COMPONENT_DEBUG_MSG("buildDiscoveryMessageEND\n");
        return discoveryString;
    }

    bool HAComponent::onHAMessage(const char *topic, const byte *p_pPayload, const unsigned int length)
    {
        HALIB_COMPONENT_DEBUG_MSG("onHAMessage\n");
        bool match = false;
        int index = 0;
        HAComponentProperty *pComponentProperty = NULL;

        while (index < mActions.getSize())
        {
            pComponentProperty = (HAComponentProperty *)mActions.get(index);
            const char *value = pComponentProperty->getValue();

            // Handle relative topics (starting with ~)
            if ('~' == *value)
            {
                const char *componentRootTopic = getPropertyValue(PROP_ROOT_TOPIC);

                if (componentRootTopic != NULL && strncmp(componentRootTopic, topic, strlen(componentRootTopic)) == 0)
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
        HALIB_COMPONENT_DEBUG_MSG("onHAMessageEND\n");
        return match;
    }

    void HAComponent::_onHAConnect()
    {
        HALIB_COMPONENT_DEBUG_MSG("_onHAConnect\n");
        if (isFirstHAConnection)
        {
            m_pNode->postDiscoveryMessage(this);
            isFirstHAConnection = false;
        }
        onHAConnect();
        HALIB_COMPONENT_DEBUG_MSG("_onHAConnectEND\n");
    }

    void HAComponent::onHAConnect()
    {
        // To be implemented by user if needed
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
        HALIB_COMPONENT_DEBUG_MSG("addAction\n");
        mActions.append(p_pProperty);

        if (NULL != m_pNode)
        {
            int pathLen = getTopicPath(p_pProperty->getValue());
            char topic[pathLen + 1];
            getTopicPath(p_pProperty->getValue(), topic);

            m_pNode->registerToHA(topic);
        }
        HALIB_COMPONENT_DEBUG_MSG("addActionEND\n");
    }

    HAComponentProperty *HAComponent::addProperty(HAComponentPropertyKey pName, const char *pValue)
    {
        HALIB_COMPONENT_DEBUG_MSG("addProperty %d: %s\n", pName, pValue ? pValue : "NULL");

        // Create new property.
        // With previous optimizations, this will NOT allocate RAM if pValue is in Flash (PROGMEM).
        HAComponentProperty *pProperty = new HAComponentProperty(pName, pValue);

        mProperties.append(pProperty);

        HALIB_COMPONENT_DEBUG_MSG("addPropertyEND\n");
        return pProperty;
    }

    char *HAComponent::buildRootTopic(const char *pNodeId)
    {
        HALIB_COMPONENT_DEBUG_MSG("buildRootTopic\n");
        PGM_P typePtr = (PGM_P)pgm_read_ptr(&componentTypeTag[m_ComponentType]);

        int topicLength = 4 + 13 + strlen_P(typePtr) + strlen(pNodeId) + strlen(getPropertyValue(PROP_NAME)) + 8;

        char *topic = (char *)malloc(topicLength + 1);
        if (topic != NULL)
        {
            topic[topicLength] = 0;
            sprintf_P(topic, ROOT_TOPIC_TEMPLATE, "homeassistant", typePtr, pNodeId, getPropertyValue(PROP_NAME), m_ComponentId);
        }

        HALIB_COMPONENT_DEBUG_MSG("buildRootTopicEND\n");
        return topic;
    }

    int HAComponent::getTopicPath(const char *topic, char *out)
    {
        // HALIB_COMPONENT_DEBUG_MSG("getTopicPath\n"); // Commented out to reduce serial spam
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
        // HALIB_COMPONENT_DEBUG_MSG("getTopicPathEND\n");
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
        // --- OPTIMIZATION ---
        // Pass NULL instead of "" to avoid any temporary allocation in HAComponentProperty constructor.
        // We just need the 'name' (Key) to match.
        HAComponentProperty propertyToFind(name, NULL);

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
                    // If not found and fallback requested, create it (Lazy Loading)
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
        // Base implementation does nothing
    }

} // namespace HALIB_NAMESPACE