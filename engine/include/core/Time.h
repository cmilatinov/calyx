#pragma once

namespace Calyx {

    using TimeT = float;

    class Time {

        friend class Application;

    public:
        static TimeT s_timeScale;

        static TimeT DeltaTime() { return s_deltaTime; }
        static TimeT FixedDeltaTime() { return s_fixedDeltaTime; }
        static TimeT CurrentTime() { return s_time; }

    private:
        static TimeT s_deltaTime;
        static TimeT s_fixedDeltaTime;
        static TimeT s_time;

    protected:
        static void UpdateTime();

    };

}

