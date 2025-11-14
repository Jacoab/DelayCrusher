#pragma once

#include <cmath>
#include <memory>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#include "concepts/NoiseGenType.h"
#include "processors/BoxMullerNoise.h"

namespace glos::clcr 
{

constexpr std::string SAMPLE_RATE_REDUX_DIAL_ID = "SAMPLE_RATE_REDUX";
constexpr std::string SAMPLE_RATE_REDUX_DIAL_TEXT = "Sample Rate Reduction";
constexpr std::string BIT_DEPTH_DIAL_ID = "BIT_DEPTH";
constexpr std::string BIT_DEPTH_DIAL_TEXT = "Bit Depth";

/**
 * @brief Bit crusher audio effect that reduces the sample rate and bit depth of an audio signal.
 * This effect can be used as a JUCE DSP processor in an audio processing chain.
 * 
 * @tparam NoiseGen Provides noise samples that can be added to the processed signal.
 */
class BitCrusher : public juce::dsp::ProcessorBase
{
public:
    /**
     * @brief Default constructor.
     * 
     * Initializes with a sample rate reduction of 0, a bit depth of 32, and a noise amount of 0.
     */
    BitCrusher() = default;

    /**
     * @brief Set the sample rate reduction parameter.
     * 
     * @param sampleRateRedux Sample rate reduction parameter
     */
    void setSampleRateReduxParam(std::atomic<float>* sampleRateReduxParam) noexcept;

    /**
     * @brief Get the sample rate reduction of the bit crusher.
     * 
     * @return float Sample rate reduction.
     */
    float getSampleRateRedux() const noexcept;

    /**
     * @brief Set parameter representing the number of bits used to represent each sample after processing.
     * 
     * @param bitDepthParam Paremeter for the number of bits used to represent each sample after processing.
     */
    void setBitDepthParam(std::atomic<float>* bitDepthParam) noexcept;

    /**
     * @brief Get the number of bits used to represent each sample after processing.
     * 
     * @return float The number of bits used to represent each sample after processing.
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

private:
    /**
     * @brief Quantize the sample to the specified bit depth.
     * 
     * @param sample Sample to be quantized.
     * @return float Quantized sample.
     */
    float quantize(float sample) const;

    float m_heldSample = 0.0f; /**< Current sample that is held and re-used for m_sampleRateRedux iterations */

    std::atomic<float>* m_sampleRateRedux; /**< Amount the sample rate will be reduced by */
    std::atomic<float>* m_bitDepth; /**< Number of bits used to represent each sample after processing */
};

} // namespace glos::clcr