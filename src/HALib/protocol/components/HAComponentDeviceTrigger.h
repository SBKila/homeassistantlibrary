#pragma once

#include <Arduino.h>
#include "HAComponent.h"

namespace HALIB_NAMESPACE
{
    class HAComponentDeviceTrigger : public HAComponent
    {
    public:
        // Constructeur
        HAComponentDeviceTrigger(const char *name);

        // Méthodes
        void trigEvent();
        virtual void onHAConnect();

    protected:
        // Cette fonction est définie à l'intérieur de la classe ("in-class definition").
        // Elle est implicitement "inline", donc elle ne cause pas d'erreur de link.
        // On peut la laisser ici ou la déplacer dans le cpp, au choix. Ici je la laisse pour simplifier.
        void _executeAction(HAComponentProperty *p_pAction, byte *payload, unsigned int length) {};
    };

} // namespace HALIB_NAMESPACE