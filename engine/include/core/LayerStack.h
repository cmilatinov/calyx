#pragma once

#include "core/ILayer.h"

namespace Calyx {

    class LayerStack {

    public:
        LayerStack() = default;
        ~LayerStack();

        void PushOverlay(ILayer* layer);
        void PushUnderlay(ILayer* layer);
        void EmplaceLayer(uint32 index, ILayer* layer);
        void PopLayer(ILayer* layer);

        List<ILayer*>::iterator begin() { return m_layers.begin(); }
        List<ILayer*>::iterator end() { return m_layers.end(); }
        List<ILayer*>::reverse_iterator rbegin() { return m_layers.rbegin(); }
        List<ILayer*>::reverse_iterator rend() { return m_layers.rend(); }

        List<ILayer*>::const_iterator begin() const { return m_layers.begin(); }
        List<ILayer*>::const_iterator end() const { return m_layers.end(); }
        List<ILayer*>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
        List<ILayer*>::const_reverse_iterator rend() const { return m_layers.rend(); }

    private:
        List<ILayer*> m_layers;

    };

}