#pragma once

#include "assets/AssetRegistry.h"
#include "render/objects/Texture2D.h"

namespace Calyx::Editor {

    class ContentBrowserPanel {

    public:
        explicit ContentBrowserPanel(const String& basePath);

        void Draw();

    private:
        Texture2D* m_objFileTexture;
        Texture2D* m_folderTexture;
        Path m_currentDirectory;
        Path m_currentRelativePath;
        float m_thumbnailSize = 120;
        float m_padding = 8;
        float m_zoomScale = 1.0f;

        void SetDirectory(const Path& path);

    };


}