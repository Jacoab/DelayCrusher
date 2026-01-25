#pragma once

#include <cmath>
#include <memory>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#include "concepts/NoiseGenType.h"
#include "processors/BoxMullerNoise.h"
#include "processors/SynchronizedProcessor.h"
#include "ProcessorParam.h"

namespace glos::clcr 
{

/**
 * @brief Bit crusher audio effect that reduces the sample rate and bit depth of an audio signal.
 * This effect is a SynchronizedProcessor based class that will take an incoming
 * audio signal in a juce::ProcessorChain and reduce the sample rate by holding samples for a 
 * specified number of iterations and reduce the bit depth by quantizing the samples to a 
 * specified number of bits.
 * 
 * @tparam NoiseGen Provides noise samples that can be added to the processed signal.
 */
class BitCrusher : public SynchronizedProcessor
{
public:
    /**
     * @brief Default constructor.
     * 
     * Initializes with a sample rate reduction of 0, a bit depth of 32, and a noise amount of 0.
     */
    BitCrusher();

    /**
     * @brief Set the sample rate reduction.
     * 
     * @param sampleRateRedux Sample rate reduction
     */
    void setSampleRateRedux(float sampleRateRedux) noexcept;

    /**
     * @brief Get the sample rate reduction of the bit crusher.
     * 
     * @return Sample rate reduction.
     */
    float getSampleRateRedux() const noexcept;

    /**
     * @brief Set the number of bits used to represent each sample after processing.
     * 
     * @param bitDepth Number of bits used to represent each sample after processing.
     */
    void setBitDepth(float bitDepth) noexcept;

    /**
     * @brief Get the number of bits used to represent each sample after processing.
     * 
     * @return The number of bits used to represent each sample after processing.
     */
    float getBitDepth() const noexcept;

    /**
     * @brief This method is required by DSP ProcessorBase child classes.  It is not currently used
     * by this class.
     * 
     * @param spec 
     */
    void prepare (const juce::dsp::ProcessSpec& spec) override;
    
    /**
     * @brief Process the audio context by reducing the sample rate and bit depth of the audio signal.
     * 
     * @param context Context information containing input and output audio blocks.
     */
    void process (const juce::dsp::ProcessContextReplacing<float>& context) override;

    /**
     * @brief Resets the bit crusher state.  This clears the held sample.
     * 
     */
    void reset() override;

    /**
     * @brief Register the processor parameter listeners with the given AudioProcessorValueTreeState.
     * 
     * @param apvts 
     */
    void registerParameters(juce::AudioProcessorValueTreeState& apvts) override;

    static constexpr char SAMPLE_RATE_REDUX_DIAL_ID[] = "SAMPLE_RATE_REDUX"; /**< Parameter ID for sample rate reduction dial */
    static constexpr char SAMPLE_RATE_REDUX_DIAL_TEXT[] = "Redux"; /**< Parameter text for sample rate reduction dial */
    static constexpr char BIT_DEPTH_DIAL_ID[] = "BIT_DEPTH"; /**< Parameter ID for bit depth dial */
    static constexpr char BIT_DEPTH_DIAL_TEXT[] = "Bit Depth"; /**< Parameter text for bit depth dial */

private:
    /**
     * @brief Quantize the sample to the specified bit depth.
     * 
     * @param sample Sample to be quantized.
     * @return Quantized sample.
     */
    float quantize(float sample) const;

    float m_heldSample = 0.0f; /**< Current sample that is held and re-used for m_sampleRateRedux iterations */

    ProcessorParam<
        SAMPLE_RATE_REDUX_DIAL_ID,
        SAMPLE_RATE_REDUX_DIAL_TEXT,
        float
    > m_sampleRateRedux; /**< Amount the sample rate will be reduced by */
    ProcessorParam<
        BIT_DEPTH_DIAL_ID,
        BIT_DEPTH_DIAL_TEXT,
        float
    > m_bitDepth; /**< Number of bits used to represent each sample after processing */
};

} // namespace glos::clcr