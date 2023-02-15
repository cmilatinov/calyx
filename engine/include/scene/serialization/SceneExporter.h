#pragma once

#include "scene/Scene.h"
#include "scene/serialization/SceneFormat.h"

namespace Calyx {

    class CALYX_API SceneExporter {

    public:
        explicit SceneExporter(const Scene& scene);

        void Save(const String& file);

    private:
        template<typename T>
        static inline constexpr const char* CharPtrCast(const T* ptr) {
            return reinterpret_cast<const char*>(ptr);
        }

    private:
        const Scene& m_scene;

    };

}