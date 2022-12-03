#pragma once

namespace Calyx {

    using TimeT = double;

    class Time {
    CX_SINGLETON(Time);

        friend class Application;

    public:
        Time() = default;

    public:
        static void SetTimeScale(TimeT timeScale) { s_instance->m_timeScale = timeScale; }

        template<typename T = TimeT>
        static T TimeScale() { return s_instance->m_timeScale; }
        template<typename T = TimeT>
        static T DeltaTime() { return s_instance->m_deltaTime; }
        template<typename T = TimeT>
        static T FixedDeltaTime() { return s_instance->m_fixedDeltaTime; }
        template<typename T = TimeT>
        static T CurrentTime() { return s_instance->m_time; }

        CX_SINGLETON_EXPOSE_METHOD(_Timer, TimeT Timer(const String& name), name);
        CX_SINGLETON_EXPOSE_METHOD(_ResetTimer, void ResetTimer(const String& name), name);

    private:
        TimeT m_timeScale = 1.0f;
        TimeT m_deltaTime = 0.0f;
        TimeT m_fixedDeltaTime = 0.0f;

        Chrono::time_point<Clock> m_lastTime;
        TimeT m_time = 0.0f;
        TimeT m_staticTime = 0.0f;

        Map<String, TimeT> m_timerMap;

        void UpdateTime();

        TimeT _Timer(const String& name);
        void _ResetTimer(const String& name);

    };

}

