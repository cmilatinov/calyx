#pragma once

namespace Calyx {

    using TimeType = double;

    class CALYX_API Time {
    CX_SINGLETON(Time);

        friend class Application;

    public:
        Time() = default;

    public:
        static void SetTimeScale(TimeType timeScale) { s_instance->m_timeScale = timeScale; }

        template<typename T = TimeType>
        static T TimeScale() { return s_instance->m_timeScale; }
        template<typename T = TimeType>
        static T DeltaTime() { return s_instance->m_deltaTime; }
        template<typename T = TimeType>
        static T StaticDeltaTime() { return s_instance->m_staticDeltaTime; }
        template<typename T = TimeType>
        static T CurrentTime() { return s_instance->m_time; }

        CX_SINGLETON_EXPOSE_METHOD(_Timer, TimeType Timer(const String& name), name);
        CX_SINGLETON_EXPOSE_METHOD(_ResetTimer, void ResetTimer(const String& name), name);

    private:
        TimeType m_timeScale = 1.0f;
        TimeType m_deltaTime = 0.0f;
        TimeType m_staticDeltaTime = 0.0f;

        Chrono::time_point<Clock> m_lastTime = Clock::now();
        TimeType m_time = 0.0f;
        TimeType m_staticTime = 0.0f;

        Map<String, TimeType> m_timerMap;

        void UpdateTime();

        TimeType _Timer(const String& name);
        void _ResetTimer(const String& name);

    };

}

