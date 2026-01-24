#include "HAAdapter.h"

namespace HALIB_NAMESPACE
{
    HAAdapter::HAAdapter(const char *name, uint8_t ioReference)
    {
        // Utilisation de strncpy pour éviter les dépassements de tampon
        strncpy(m_Name, name, 20);
        // Sécurité : on force le caractère de fin de chaîne au cas où 'name' > 19 chars
        m_Name[19] = '\0';
        m_pinNumber = ioReference;
    }

    HAAdapter::~HAAdapter()
    {
        // Destructeur vide
    }

    const char *HAAdapter::getName() const
    {
        return m_Name;
    }

    void HAAdapter::setup()
    {
        _setup();
        suspend(false);
    }

    bool HAAdapter::operator!=(const IHAAdapter &other)
    {
        return (strcmp(other.getName(), m_Name) != 0);
    }

    bool HAAdapter::operator==(const IHAAdapter &other)
    {
        return (strcmp(other.getName(), m_Name) == 0);
    }

    void HAAdapter::loop()
    {
        // Implémentation par défaut vide
    }

    void HAAdapter::suspend(boolean state)
    {
        // Implémentation par défaut vide
    }

} // namespace HALIB_NAMESPACE