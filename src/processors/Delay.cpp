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
    if (m_delayLine1Active)
        m_delayLine2.setDelay(delayTime);
    else
        m_delayLine1.setDelay(delayTime);
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

void Delay::prepare(const juce::dsp::ProcessSpec &spec)
{
    m_sampleRate = spec.sampleRate;

    m_delayLine1.prepare(spec);
    m_delayLine1.setMaximumDelayInSamples(static_cast<int>(20.0 * m_sampleRate));
    m_delayLine1.setDelay(getDelayTimeInSamples());

    m_delayLine2.prepare(spec);
    m_delayLine2.setMaximumDelayInSamples(static_cast<int>(20.0 * m_sampleRate));
    m_delayLine2.setDelay(getDelayTimeInSamples());
}

void Delay::process(const juce::dsp::ProcessContextReplacing<float> &context)
{
    auto &block = context.getOutputBlock();
    auto numChannels = block.getNumChannels();
    auto numSamples = block.getNumSamples();

    float targetDelayTimeSamples = static_cast<float>(getDelayTimeInSamples());
    m_smoothedDelayTime.setTargetValue(targetDelayTimeSamples);

    for (size_t sample = 0; sample < numSamples; ++sample)
    {
        float currentDelayTimeSamples = m_smoothedDelayTime.getNextValue();

        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* channelData = block.getChannelPointer(channel);
            auto inputSample = channelData[sample];

            std::unique_lock<std::mutex> lock(m_DelayLineMutex);
            float outputSample = 0.0f;
            if (m_delayLine1Active)
            {
                m_delayLine1.pushSample(static_cast<int>(channel), inputSample);
                outputSample = m_delayLine1.popSample(static_cast<int>(channel), currentDelayTimeSamples, true);
            }
            else
            {
                m_delayLine2.pushSample(static_cast<int>(channel), inputSample);
                outputSample = m_delayLine2.popSample(static_cast<int>(channel), currentDelayTimeSamples, true);
            }
            lock.unlock();

            channelData[sample] = inputSample + outputSample * getDryWet();
        }
    }
}

void Delay::reset()
{
    m_delayLine1.reset();
    m_delayLine2.reset();
    m_delayLine1Active = true;
}

void Delay::registerParameters(juce::AudioProcessorValueTreeState &apvts)
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