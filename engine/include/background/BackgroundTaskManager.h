#pragma once

#include "background/ThreadPool.h"

namespace Calyx::Editor {

    class BackgroundTaskManager {
        CX_SINGLETON(BackgroundTaskManager);

    public:
        BackgroundTaskManager() = default;

        void BuildAssemblies();
        void Draw();

        ThreadPool& GetThreadPool() { return m_threadPool; }

        void SetProgress(float progress) { m_infoMessage = progress; }
        void SetInfoMessage(const String& msg) { m_infoMessage = msg; }

    private:
        ThreadPool m_threadPool;
        Future<void> m_buildResult;
        bool m_openPopupNextFrame = false;
        String m_infoMessage;
        float m_progress = 0.0f;

    };

}