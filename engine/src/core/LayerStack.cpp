#include "core/LayerStack.h"

namespace Calyx {

    LayerStack::~LayerStack() {
        for (auto* layer: m_layers) {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::PushOverlay(ILayer* layer) {
        m_layers.emplace_back(layer);
        layer->OnAttach();
    }

    void LayerStack::PushUnderlay(ILayer* layer) {
        m_layers.emplace(m_layers.begin(), layer);
        layer->OnAttach();
    }

    void LayerStack::EmplaceLayer(uint32 index, ILayer* layer) {
        m_layers.emplace(m_layers.begin() + index, layer);
        layer->OnAttach();
    }

    void LayerStack::PopLayer(ILayer* layer) {
        auto it = std::find(m_layers.begin(), m_layers.end(), layer);
        if (it != m_layers.end()) {
            m_layers.erase(it);
        }
    }

}