#include "BoxMullerNoise.h"

namespace glos::clcr
{

BoxMullerNoise::BoxMullerNoise(int numChannels, int sampleRate) :
    m_usePrecomputedSample(false),
    m_precomputedSample(0.0f),
    m_random(),
    m_samples(numChannels, sampleRate)
{
}

float BoxMullerNoise::nextSample() noexcept
{
    // Since the Box-Muller transform generates two independent samples, check to see if
    // we already have a pre-computed sample to return.
    if (m_usePrecomputedSample) 
    {
        m_usePrecomputedSample = false;
        return m_precomputedSample;
    }

    float u, v, s;
    do 
    {
        u = 2.0f * m_random.nextFloat() - 1.0f;
        v = 2.0f * m_random.nextFloat() - 1.0f;
        s = u * u + v * v;
    } while (s >= 1.0f || s == 0.0f);

    auto multiplier = std::sqrt(-2.0f * std::log(s) / s);
    m_precomputedSample = v * multiplier;
    m_usePrecomputedSample = true;
    return u * multiplier;
}

juce::AudioBuffer<float> BoxMullerNoise::nextNSamples(int n) noexcept
{
    jassert (n > 0 && n <= m_samples.getNumSamples());

    m_samples.clear();
    for (auto channel = 0; channel < m_samples.getNumChannels(); ++channel)
    {
        auto* samples = m_samples.getWritePointer(channel);
        for (auto i = 0; i < n; ++i)
        {
            samples[i] = nextSample();
        }
    }
    return m_samples;
}

}