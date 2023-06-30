#pragma once

namespace Calyx::Editor {

    struct ProjectInfo {
        String name;
        Path projectFile;
        struct {
            Path project;
            Path assets;
            Path build;
            Path cmake;
        } directories;
    };

}