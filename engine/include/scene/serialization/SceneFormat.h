#pragma once

#define CX_SCENE_VERSION_MAJOR 1
#define CX_SCENE_VERSION_MINOR 0

#define CX_SCENE_MAGIC 0x0210512578042069

namespace Calyx {

    struct SceneHeader {
        uint64 magic;
        struct {
            uint32 major;
            uint32 minor;
        } version;
        uint64 objectCount;
    };

};