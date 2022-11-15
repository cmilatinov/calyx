#pragma once

#include "events/Event.h"

namespace Calyx {

    class ILayer {

    public:
        virtual ~ILayer() = default;

        virtual void OnAttach() {};
        virtual void OnDetach() {};
        virtual void OnUpdate() {};
        virtual void OnGUI() {};
        virtual void OnEvent(Event& event) {};

        virtual String GetName() const = 0;

    };

}