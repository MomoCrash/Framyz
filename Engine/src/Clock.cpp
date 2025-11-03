#include "Clock.h"



Clock::Clock(): timeScale(1.0f)
{
    mLastFrameTime = std::chrono::high_resolution_clock::now();
}

float Clock::GetDeltaTime() const
{
    return (mDeltaTime / 1000000000.0f) * timeScale;
}

float Clock::GetUnscaledDeltaTime() const {
    return (mDeltaTime / 1000000000.0f);
}

float Clock::GetFixedDeltaTime() const
{
    return m_fixedDeltaTime * timeScale;
}

float Clock::GetUnscaledFixedDeltaTime() {
    return m_fixedDeltaTime;
}

void Clock::UpdateTime()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    mDeltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - mLastFrameTime).count();
    mLastFrameTime = currentTime;
}

float Clock::GetFPS()
{
    return 1.0f / GetDeltaTime();
}

void Clock::SetTimeScale(float scale)
{
    timeScale = scale;
}