#pragma once

#include <chrono>

class Clock
{
public:
    Clock();

    float GetDeltaTime() const;
    float GetUnscaledDeltaTime() const;
    
    float GetFixedDeltaTime() const;
    static float GetUnscaledFixedDeltaTime();
    
    void UpdateTime();
    float GetFPS();
    void SetTimeScale(float scale);

private:
    std::chrono::high_resolution_clock::time_point mLastFrameTime;
    float timeScale;
    unsigned long long mDeltaTime = 0ULL;
    static constexpr float m_fixedDeltaTime = 1.0f/60.0f;
};