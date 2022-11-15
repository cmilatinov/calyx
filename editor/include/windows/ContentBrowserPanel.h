#pragma once

#include <imgui.h>

namespace Calyx::Editor {

    class ContentBrowserPanel {

    public:
        ContentBrowserPanel(const String& basePath);

        void Draw();

    private:
        Path m_currentDirectory;
        float m_thumbnailSize = 128;
        float m_padding = 8;
        float m_zoomScale = 1.0f;

    };


}