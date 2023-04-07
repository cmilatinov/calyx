#pragma once

#include "scene/serialization/SceneFormat.h"
#include "scene/Scene.h"

namespace Calyx {

    class CALYX_API SceneImporter {

    public:
        Scope<Scene> Load(const String& file);

    private:
        template<typename T>
        static inline void Read(std::istream& stream, T& dest) {
            stream.read(reinterpret_cast<char*>(&dest), sizeof(dest));
        }

    private:


    };


}