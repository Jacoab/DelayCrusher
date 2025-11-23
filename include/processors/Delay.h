#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#include "processors/SynchronizedProcessor.h"
#include "ProcessorParam.h"

namespace glos::clcr
{

/**
 * @class Delay
 * @brief Delay audio effect class.  This effect is a SynchronizedProcessor
 * based class that will take an incoming audio signal in a juce::ProcessorChain
 * and apply a delay to it.
 * 
 */
class Delay : public SynchronizedProcessor
{
public:
    /**
     * @brief Construct a new Delay object.
     * 
     */
    Delay();

    /**
     * @brief Sets the delay time parameter in milliseconds.
     * 
     * @param delayTime delay time in milliseconds.
     */
    void setDelayTime(float delayTime);

    /**
     * @brief Gets the delay time parameter value in milliseconds.
     * 
     * @return float delay time parameter value in milliseconds.
     */
    float getDelayTime() const;

    /**
     * @brief Set the dry wet mix parameter.
     * 
     * @param dryWet Dry/Wet mix parameter.
     */
    void setDryWet(float dryWet);

    /**
     * @brief Get the current dry wet mix parameter value.
     * 
     * @return float Current dry wet mix parameter value.
     */
    float getDryWet() const;

    /**
     * @brief Initializes the delay line and sets the sample rate.
     * 
     * @param spec Process specification containing audio host info.
     */
    void prepare (const juce::dsp::ProcessSpec& spec) override;
    
    /**
     * @brief Takes the audio context and processes incoming audio with the delay effect.
     * The samples in the audio context output block are pushed onto the delay line and
     * the delayed samples are popped from the delay line and mixed with the input samples.
     * 
     * @param context Context information containing input and output audio blocks.
     */
    void process (const juce::dsp::ProcessContextReplacing<float>& context) override;

    /**
     * @brief Resets the delay line.
     */
    void reset() override;

    /**
     * @brief Registers the delay time and dry/wet parameters with the given
     * AudioProcessorValueTreeState.
     * 
     * @param apvts Processor's AudioProcessorValueTreeState.
     */
    void registerParameters(juce::AudioProcessorValueTreeState& apvts) override;

    static constexpr char DELAY_TIME_DIAL_ID[] = "DELAY_TIME"; /**< Delay time parameter ID. */
    static constexpr char DELAY_TIME_DIAL_TEXT[] = "Delay Time (ms)"; /**< Delay time parameter text. */
    static constexpr char DRY_WET_DIAL_ID[] = "DELAY_DRY_WET"; /**< Dry/Wet mix parameter ID. */
    static constexpr char DRY_WET_DIAL_TEXT[] = "Delay Dry/Wet"; /**< Dry/Wet mix parameter text. */

private:
    /**
     * @brief Gets the delay time as the number of samples needed to produce that delay.
     * 
     * @return int delay time in samples.
     */
    int getDelayTimeInSamples() const;

    juce::dsp::DelayLine<float> m_delayLine; /**< Delay line processor. */
    double m_sampleRate = 44100.0; /**< Sample rate of the audio host. */

    ProcessorParam<
        DELAY_TIME_DIAL_ID,
        DELAY_TIME_DIAL_TEXT,
        float
    > m_delayTime; /** Delay time in milliseconds. */
    ProcessorParam<
        DRY_WET_DIAL_ID,
        DRY_WET_DIAL_TEXT,
        float
    > m_dryWet; /** Dry/Wet mix parameter. */
};

}