#include "Delay.h"

namespace glos::clcr
{

void Delay::setDelayTime(std::atomic<float>* delayTime)
{
    m_delayTime = delayTime;
}

float Delay::getDelayTime() const
{
    return m_delayTime ? m_delayTime->load() : 0.0f;
}

void Delay::prepare (const juce::dsp::ProcessSpec& spec)
{
    m_sampleRate = spec.sampleRate;

    m_delayLine.prepare(spec);
    m_delayLine.setMaximumDelayInSamples(static_cast<int>(20.0 * m_sampleRate)); // 20 second max delay
    m_delayLine.setDelay(getDelayTimeInSamples());
}
    
void Delay::process (const juce::dsp::ProcessContextReplacing<float>& context)
{
    auto& block = context.getOutputBlock();
    auto numChannels = block.getNumChannels();
    auto numSamples = block.getNumSamples();

    for (size_t channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);

        for (size_t sample = 0; sample < numSamples; ++sample)
        {
            auto inputSample = channelData[sample];
            m_delayLine.pushSample(static_cast<int>(channel), inputSample);

            auto outputSample = m_delayLine.popSample(static_cast<int>(channel), getDelayTimeInSamples(), true);
            channelData[sample] = inputSample + outputSample*0.3f;
        }
    }
}

void Delay::reset()
{
    m_delayLine.reset();
}

int Delay::getDelayTimeInSamples() const
{
    return static_cast<int>(getDelayTime() * m_sampleRate);
}

}