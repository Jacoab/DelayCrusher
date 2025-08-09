#include "TestNoise.h"

TestNoise::TestNoise() : 
    m_noiseSample(0.01f),
    m_noiseModifier([]() { return 1.0f; })
{
}

TestNoise::TestNoise(float noiseSample) : 
    m_noiseSample(noiseSample),
    m_noiseModifier([]() { return 1.0f; })
{
}

TestNoise::TestNoise(float noiseSample, std::function<float()> noiseChange) : 
    m_noiseSample(noiseSample),
    m_noiseModifier(std::move(noiseChange))
{
}

float TestNoise::nextSample() noexcept
{
    auto noiseChange = m_noiseModifier();
    return m_noiseSample * noiseChange;
}  