#include "processors/Delay.h"

namespace glos::clcr
{

Delay::Delay() :
    m_delayTime(0.0f),
    m_dryWet(0.5f)
{
}

void Delay::setDelayTime(float delayTime)
{
    m_delayTime.store(delayTime);
}

float Delay::getDelayTime() const
{
    return m_delayTime.load();
}

void Delay::setDryWet(float dryWet)
{
    m_dryWet.store(dryWet);
}

float Delay::getDryWet() const
{
    return m_dryWet.load();
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
            channelData[sample] = inputSample + outputSample * getDryWet();
        }
    }
}

void Delay::reset()
{
    m_delayLine.reset();
}

void Delay::registerParameters(juce::AudioProcessorValueTreeState& apvts)
{
    apvts.addParameterListener(DELAY_TIME_DIAL_ID, &m_delayTime);
    apvts.addParameterListener(DRY_WET_DIAL_ID, &m_dryWet);
}

int Delay::getDelayTimeInSamples() const
{
    double delaySeconds = static_cast<double>(getDelayTime()) / 1000.0;
    return static_cast<int>(std::round(delaySeconds * m_sampleRate));
}

}