#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

/**
 * @brief Simple noise generator for testing purposes.  A noise generation
 * function can be provided to modify the noise sample returned by nextSample() in
 * a predictable way.
 * 
 */
class ConstNoise
{
public:
    ConstNoise();
    ConstNoise(float noiseSample);

    float nextSample() noexcept;
    juce::AudioBuffer<float>& nextNSamples(int n) noexcept;
private:
    float m_noiseSample;
    juce::AudioBuffer<float> m_noiseBuffer; // Buffer to hold the noise samples
};