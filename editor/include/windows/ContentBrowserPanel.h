#pragma once

#include "assets/AssetRegistry.h"
#include "render/objects/Texture2D.h"
#include "ui/Definitions.h"

namespace Calyx::Editor {

    class ContentBrowserPanel {

    public:
        explicit ContentBrowserPanel(const Path& basePath);
        explicit ContentBrowserPanel(const String& basePath);

        void Draw();

    private:
        Ref<Texture2D> m_objFileTexture;
        Ref<Texture2D> m_folderTexture;
        Path m_rootDirectory;
        Path m_currentDirectory;
        float m_thumbnailSize = 120;
        float m_padding = 8;
        float m_zoomScale = 1.0f;

        void SetDirectory(const Path& path);

    };


}