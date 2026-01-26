#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace dlcr
{

/**
 * @class SynchronizedProcessor
 * @brief Abstract base class for DSP processors that are synchronized with 
 * AudioProcessorValueTreeState parameters.
 */
struct SynchronizedProcessor : public juce::dsp::ProcessorBase
{
    virtual ~SynchronizedProcessor() = default;

    /**
     * @brief Prepare the processor with the given process specification.
     * 
     * @param spec Process specification containing audio host info.
     */
    virtual void prepare (const juce::dsp::ProcessSpec& spec) = 0;

    /**
     * @brief Process the audio context.
     * 
     * @param context Context information containing input and output audio blocks.
     */
    virtual void process (const juce::dsp::ProcessContextReplacing<float>& context) = 0;

    /**
     * @brief Reset the processor state.
     * 
     */
    virtual void reset() = 0;

    /**
     * @brief Register the processor parameter listeners with the given AudioProcessorValueTreeState.
     * 
     * @param apvts Processor's AudioProcessorValueTreeState.
     */
    virtual void registerParameters(juce::AudioProcessorValueTreeState& apvts) = 0;

};

} // namespace dlcr