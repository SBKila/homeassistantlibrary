#include "HAComponentDeviceTrigger.h"
#include "../../tools/debug.h"

namespace HALIB_NAMESPACE
{
    // Implémentation du Constructeur
    HAComponentDeviceTrigger::HAComponentDeviceTrigger(const char *name) : HAComponent(name, DEVICE_TRIGGER)
    {
        addProperty(PROP_AUTOMATION_TYPE, "trigger");
        addProperty(PROP_TOPIC_TOPIC);
        addProperty(PROP_TYPE, "button_short_press");
        addProperty(PROP_STYPE, "button1");
    }

    // Implémentation de onHAConnect
    void HAComponentDeviceTrigger::onHAConnect()
    {
        // Vide par défaut
    }

    // Implémentation de trigEvent
    void HAComponentDeviceTrigger::trigEvent()
    {
        // Attention : l'allocation dynamique sur la stack (VLA) n'est pas standard C++
        // mais souvent supportée par GCC.
        // Si getTopicPath retourne une valeur, assurez-vous qu'elle est valide.
        char topic[getTopicPath(PROP_TOPIC_TOPIC) + 1];

        getTopicPath(PROP_TOPIC_TOPIC, topic);

        if (NULL != m_pNode)
        {
            m_pNode->postMessage(topic, "1", false);
        }
        else
        {
            HALIB_COMPONENT_DEBUG_MSG("Node not configured\n");
        }
    }

} // namespace HALIB_NAMESPACE