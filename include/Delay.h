#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

namespace glos::clcr
{

constexpr std::string DELAY_TIME_DIAL_ID = "DELAY_TIME";
constexpr std::string DELAY_TIME_DIAL_TEXT = "Delay Time (ms)";

class Delay : public juce::dsp::ProcessorBase
{
public:
    /**
     * @brief Construct a new Delay object.
     * 
     */
    Delay() = default;

    /**
     * @brief Sets the delay time in milliseconds.
     * 
     * @param delayTime delay time in milliseconds.
     */
    void setDelayTime(std::atomic<float>* delayTime);

    /**
     * @brief Gets the delay time in milliseconds.
     * 
     * @return float delay time in milliseconds.
     */
    float getDelayTime() const;

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

    void reset() override;

private:
    /**
     * @brief Gets the delay time as the number of samples needed to produce that delay.
     * 
     * @return int delay time in samples.
     */
    int getDelayTimeInSamples() const;

    juce::dsp::DelayLine<float> m_delayLine; /**< Delay line processor. */
    double m_sampleRate = 44100.0; /**< Sample rate of the audio host. */

    std::atomic<float>* m_delayTime = nullptr; /** Delay time in milliseconds. */
};

}