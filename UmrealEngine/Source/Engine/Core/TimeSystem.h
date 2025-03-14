#pragma once

class TimeSystem;
extern TimeSystem& Time;

class TimeSystem
{
public:
    //싱글톤용 인스턴스
    static TimeSystem Time;
private:
    TimeSystem();
    ~TimeSystem();

public:
    //엔진 접근용 네임스페이스
    struct Engine
    {
        /*엔진 Update를 제어하기 위한 함수. 매프레임 호출해야함. */
        static void TimeSystemUpdate();

        /*엔진 Fixed Update를 제어하기 위한 함수. true를 반환하면 Fixed Update를 호출하면 됨.*/
        static bool TimeSystemFixedUpdate();
    };
    friend TimeSystem::Engine;
private:
    /*엔진 Update를 제어하기 위한 함수. 매프레임 호출해야함.*/
    void TimeSystemUpdate();

    /*엔진 Fixed Update를 제어하기 위한 함수. true를 반환하면 Fixed Update를 호출하면 됨.*/
    bool TimeSystemFixedUpdate();

public:
    /*시간이 경과하는 속도를 제어합니다. */
    double timeScale = 1.0;

    /*Fixed Update가 호출되는 주기시간을 제어합니다. 기본값 : 0.02f (1초에 50번)*/
    double fixedTimeStep = 0.02;

    /*deltaTime의 최대값을 제어합니다. 반드시 fixedTimeStep 이상이어야 합니다. 기본값 : 0.333333*/
    double maximumDeltaTime = 0.33333333333333333;

public:
    /*델타 타임. 단위 : 초 */
    inline float deltaTime() const
    {
        return m_deltaTime;
    }

    /*Time Scale 영향 안받는 DeltaTime*/
    inline float unscaledDeltaTime() const
    {
        return m_unscaledDeltaTime;
    }

    /*Fixed DeltaTime 주기*/
    inline float fixedDeltaTime() const
    {
        return m_fixedDeltaTime;
    }

    /*Time Scale 영향 받는 실제 주기*/
    inline float fixedUnscaledDeltaTime() const
    {
        return m_fixedUnscaledDeltaTime;
    }

    /*게임이 시작된 이후의 총 프레임 수.*/
    inline long long frameCount() const
    {
        return m_frameCount;
    }

    /*DeltaTime으로 FPS를 계산*/
    inline const int frameRate() const
    {
        double fps = 1.0 / m_unscaledDeltaTime;
        return static_cast<int>(std::clamp(fps, 0.0, fps));
    }

    /*프로그램 시작 후 경과한 실제 시간을 초 단위로 반환합니다.*/
    inline float realtimeSinceStartup() const
    {
        return float(m_realtimeSinceStartup) / float(m_frequency.QuadPart);
    }

    /*프로그램 시작 후 경과한 실제 시간을 초 단위로 반환합니다. (정밀도 double)*/
    inline double realtimeSinceStartupAsDouble() const
    {
        return double(m_realtimeSinceStartup) / double(m_frequency.QuadPart);
    }

    /*프로그램 시작 후 경과한 시간을 초 단위로 반환합니다. (TimeScale 영향 받습니다.)*/
    inline float time() const
    {
        return float(m_time) / float(m_frequency.QuadPart);
    }

    /*프로그램 시작 후 경과한 시간을 초 단위로 반환합니다. (정밀도 double)*/
    inline double timeAsDouble() const
    {
        return double(m_time) / double(m_frequency.QuadPart);
    }

private:
    LARGE_INTEGER m_previousTime{};
    LARGE_INTEGER m_currentTime{};
    LARGE_INTEGER m_frequency{};

    LONGLONG	  m_time{};
    LONGLONG	  m_realtimeSinceStartup{};

    double		  m_deltaTime{};
    double		  m_unscaledDeltaTime{};

    double		  m_elapsedFixedTime{};
    double		  m_fixedDeltaTime{};
    double		  m_fixedUnscaledDeltaTime{};

    unsigned long long m_frameCount{};
};

