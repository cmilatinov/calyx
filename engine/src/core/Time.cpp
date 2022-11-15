#include "core/Time.h"

namespace Calyx {

    TimeT Time::s_timeScale = 1.0f;

    TimeT Time::s_deltaTime = 0.0f;
    TimeT Time::s_fixedDeltaTime = 0.0f;
    TimeT Time::s_time = 0.0f;

    void Time::UpdateTime() {
        s_time += s_deltaTime * s_timeScale;
    }

}