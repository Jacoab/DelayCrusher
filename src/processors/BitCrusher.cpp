#include "processors/BitCrusher.h"

namespace glos::clcr 
{

void BitCrusher::setSampleRateRedux(std::atomic<float>* sampleRateRedux) noexcept
{
    m_sampleRateRedux = sampleRateRedux;
}

std::atomic<float>* BitCrusher::getSampleRateRedux() const noexcept
{
    return m_sampleRateRedux;
}

void BitCrusher::setBitDepth(std::atomic<float>* bitDepth) noexcept
{
    m_bitDepth = bitDepth;
}

std::atomic<float>* BitCrusher::getBitDepth() const noexcept
{
    return m_bitDepth;
}

void BitCrusher::prepare (const juce::dsp::ProcessSpec& spec)
{

}

void BitCrusher::process (const juce::dsp::ProcessContextReplacing<float>& context)
{
    auto& block = context.getOutputBlock();
    auto numChannels = block.getNumChannels();
    auto numSamples = block.getNumSamples();
    //auto noise = m_noiseGenerator.nextNSamples(static_cast<int>(numSamples));

    auto redux = static_cast<int>(m_sampleRateRedux->load());
    for (std::size_t channel = 0; channel < numChannels; ++channel)
    {
        auto* samples = block.getChannelPointer(channel);
        //auto* noiseSamples = noise.getWritePointer(static_cast<int>(channel));

        for (std::size_t i = 0; i < numSamples; ++i)
        {
            if (static_cast<int>(i) % redux == 0)
                m_heldSample = quantize(samples[i]);

            samples[i] = m_heldSample;
        }

        //juce::FloatVectorOperations::multiply(noiseSamples, m_noiseAmount->load(), numSamples);
        //juce::FloatVectorOperations::add(samples, noiseSamples, numSamples);
    }
}

void BitCrusher::reset()
{
    m_heldSample = 0.0f;
}

float BitCrusher::quantize(float sample) const
{
    float scale = powf(2.0f, m_bitDepth->load());
    return std::floor(sample * scale) / scale;
}

}