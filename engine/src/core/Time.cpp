#include "core/Time.h"

namespace Calyx {

    CX_SINGLETON_INSTANCE(Time);

    void Time::UpdateTime() {
        auto currentTime = Clock::now();
        m_staticDeltaTime = DurationCast<Microseconds>(currentTime - m_lastTime).count() / 1e6;
        m_deltaTime = m_staticDeltaTime * m_timeScale;
        m_staticTime += m_staticDeltaTime;
        m_time += m_deltaTime;
        m_lastTime = currentTime;
    }

    TimeType Time::_Timer(const String& name) {
        return m_staticTime - m_timerMap[name];
    }

    void Time::_ResetTimer(const String& name) {
        m_timerMap[name] = m_staticTime;
    }

}