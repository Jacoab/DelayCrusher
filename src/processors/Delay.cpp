#include "processors/Delay.h"

namespace dlcr
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

void Delay::prepare(const juce::dsp::ProcessSpec &spec)
{
    m_sampleRate = spec.sampleRate;

    m_delayLine1.prepare(spec);
    m_delayLine1.setMaximumDelayInSamples(static_cast<int>(20.0 * m_sampleRate));
    
    m_delayLine2.prepare(spec);
    m_delayLine2.setMaximumDelayInSamples(static_cast<int>(20.0 * m_sampleRate));
    
    m_currentDelayTimeSamples = getDelayTimeInSamples();
    m_targetDelayTimeSamples = m_currentDelayTimeSamples;
    
    m_delayLine1.setDelay(static_cast<float>(m_currentDelayTimeSamples));
    m_delayLine2.setDelay(static_cast<float>(m_currentDelayTimeSamples));

    m_crossfade.reset(m_sampleRate, CROSSFADE_TIME_SECONDS);
    m_crossfade.setCurrentAndTargetValue(0.0f);
}

void Delay::process(const juce::dsp::ProcessContextReplacing<float> &context)
{
    auto &block = context.getOutputBlock();
    auto numChannels = block.getNumChannels();
    auto numSamples = block.getNumSamples();

    int newTargetDelay = getDelayTimeInSamples();

    if (
        newTargetDelay != m_targetDelayTimeSamples && 
        m_crossfade.getCurrentValue() == m_crossfade.getTargetValue()
    )
    {
        m_targetDelayTimeSamples = newTargetDelay;
        
        if (m_delayLine1Active)
        {
            m_delayLine2.setDelay(static_cast<float>(m_targetDelayTimeSamples));
            m_crossfade.setTargetValue(1.0f); // Crossfade to delay line 2
        }
        else
        {
            m_delayLine1.setDelay(static_cast<float>(m_targetDelayTimeSamples));
            m_crossfade.setTargetValue(0.0f); // Crossfade to delay line 1
        }
    }

    for (size_t sample = 0; sample < numSamples; ++sample)
    {
        float crossfadeValue = m_crossfade.getNextValue();
        
        if (
            crossfadeValue == m_crossfade.getTargetValue() && 
            m_currentDelayTimeSamples != m_targetDelayTimeSamples
        )
        {
            m_delayLine1Active = !m_delayLine1Active;
            m_currentDelayTimeSamples = m_targetDelayTimeSamples;
        }

        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* channelData = block.getChannelPointer(channel);
            auto inputSample = channelData[sample];

            m_delayLine1.pushSample(static_cast<int>(channel), inputSample);
            m_delayLine2.pushSample(static_cast<int>(channel), inputSample);

            float sample1 = m_delayLine1.popSample(static_cast<int>(channel));
            float sample2 = m_delayLine2.popSample(static_cast<int>(channel));
            
            float outputSample = sample1 * (1.0f - crossfadeValue) + 
                                sample2 * crossfadeValue;
            
            channelData[sample] = inputSample + outputSample * getDryWet();
        }
    }
}

void Delay::reset()
{
    m_delayLine1.reset();
    m_delayLine2.reset();
    m_delayLine1Active = true;
    m_currentDelayTimeSamples = getDelayTimeInSamples();
    m_targetDelayTimeSamples = m_currentDelayTimeSamples;
    m_crossfade.reset(m_sampleRate, CROSSFADE_TIME_SECONDS);
    m_crossfade.setCurrentAndTargetValue(0.0f);
}

void Delay::registerParameters(juce::AudioProcessorValueTreeState &apvts)
{
    apvts.addParameterListener(DELAY_TIME_DIAL_ID, &m_delayTime);
    apvts.addParameterListener(DRY_WET_DIAL_ID, &m_dryWet);
}

int Delay::getDelayTimeInSamples() const
{
    double delaySeconds = static_cast<double>(getDelayTime()) / 1000.0;
    int samples = static_cast<int>(std::round(delaySeconds * m_sampleRate));
    return std::max(1, samples);
}

}