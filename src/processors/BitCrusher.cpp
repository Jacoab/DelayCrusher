#include "processors/BitCrusher.h"

namespace glos::clcr 
{

BitCrusher::BitCrusher() :
    m_sampleRateRedux(0.0f),
    m_bitDepth(32.0f)
{
}

void BitCrusher::setSampleRateRedux(float sampleRateRedux) noexcept
{
    m_sampleRateRedux.store(sampleRateRedux);
}

float BitCrusher::getSampleRateRedux() const noexcept
{
    return m_sampleRateRedux.load();
}

void BitCrusher::setBitDepth(float bitDepthParam) noexcept
{
    m_bitDepth.store(bitDepthParam);
}

float BitCrusher::getBitDepth() const noexcept
{
    return m_bitDepth.load();
}

void BitCrusher::prepare (const juce::dsp::ProcessSpec& spec)
{

}

void BitCrusher::process (const juce::dsp::ProcessContextReplacing<float>& context)
{
    auto& block = context.getOutputBlock();
    auto numChannels = block.getNumChannels();
    auto numSamples = block.getNumSamples();

    auto redux = static_cast<int>(m_sampleRateRedux.load());
    for (std::size_t channel = 0; channel < numChannels; ++channel)
    {
        auto* samples = block.getChannelPointer(channel);

        for (std::size_t i = 0; i < numSamples; ++i)
        {
            if (static_cast<int>(i) % redux == 0)
                m_heldSample = quantize(samples[i]);

            samples[i] = m_heldSample;
        }
    }
}

void BitCrusher::reset()
{
    m_heldSample = 0.0f;
}

void BitCrusher::registerParameters(juce::AudioProcessorValueTreeState& apvts)
{
    apvts.addParameterListener(SAMPLE_RATE_REDUX_DIAL_ID, &m_sampleRateRedux);
    apvts.addParameterListener(BIT_DEPTH_DIAL_ID, &m_bitDepth);
}

float BitCrusher::quantize(float sample) const
{
    float scale = powf(2.0f, m_bitDepth.load());
    return std::floor(sample * scale) / scale;
}

}