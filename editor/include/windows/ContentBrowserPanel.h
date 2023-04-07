#pragma once

#include "assets/AssetRegistry.h"
#include "render/objects/Texture2D.h"
#include "ui/Definitions.h"

namespace Calyx::Editor {

    class ContentBrowserPanel {

    public:
        ContentBrowserPanel();

        void Draw();

    private:
        void ContextMenu();

        void NewFolder();
        void ResetNewFolder();

        void NewComponent();
        void ResetNewComponent();

    private:
        Ref<Texture2D> m_objFileTexture;
        Ref<Texture2D> m_folderTexture;
        Path m_rootDirectory;
        Path m_currentDirectory;
        float m_thumbnailSize = 120;
        float m_padding = 8;

        String m_componentNamespace = "Calyx";
        String m_componentName;
        String m_folderName;

        void SetDirectory(const Path& path);

    };


}