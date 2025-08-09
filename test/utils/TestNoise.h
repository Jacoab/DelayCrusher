#pragma once

#include <juce_core/juce_core.h>

/**
 * @brief Simple noise generator for testing purposes.  A noise generation
 * function can be provided to modify the noise sample returned by nextSample() in
 * a predictable way.
 * 
 */
class TestNoise
{
public:
    TestNoise();
    TestNoise(float noiseSample);
    TestNoise(float noiseSample, std::function<float()> noiseChange);

    float nextSample() noexcept;
private:
    float m_noiseSample;
    std::function<float()> m_noiseModifier; /**< Function to generate noise samples */
};