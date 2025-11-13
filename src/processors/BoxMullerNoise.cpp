#include "processors/BoxMullerNoise.h"

namespace glos::clcr
{

BoxMullerNoise::BoxMullerNoise(int numChannels, int sampleRate) :
    m_usePrecomputedSample(false),
    m_precomputedSample(0.0f),
    m_random(),
    m_samples(numChannels, sampleRate)
{
}

void BoxMullerNoise::setNoiseAmount(std::atomic<float>* noiseAmount) noexcept
{
    m_noiseAmount = noiseAmount;
}

std::atomic<float>* BoxMullerNoise::getNoiseAmount() const noexcept
{
    return m_noiseAmount;
}

void BoxMullerNoise::prepare(const juce::dsp::ProcessSpec& spec)
{
    auto numChannels = static_cast<int>(spec.numChannels);
    auto numSamples = static_cast<int>(spec.maximumBlockSize);
    m_samples.setSize(numChannels, numSamples); // Might need to change some of the default params here
}
    
void BoxMullerNoise::process(const juce::dsp::ProcessContextReplacing<float>& context)
{
    auto& block = context.getOutputBlock();
    auto numChannels = block.getNumChannels();
    auto numSamples = block.getNumSamples();
    auto noise = nextNSamples(static_cast<int>(numSamples));

    for (size_t channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);
        auto* noiseSamples = noise.getWritePointer(static_cast<int>(channel));
        auto noiseAmount = m_noiseAmount ? m_noiseAmount->load() : 0.0f;
        auto numValues = static_cast<int>(numSamples);

        // Scale noise by noise amount parameter
        juce::FloatVectorOperations::multiply(noiseSamples, noiseAmount, numValues);
        juce::FloatVectorOperations::add(channelData, noiseSamples, numValues);
    }
}

void BoxMullerNoise::reset()
{
    m_usePrecomputedSample = false;
    m_precomputedSample = 0.0f;
    m_samples.clear();
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