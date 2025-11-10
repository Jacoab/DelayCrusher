#pragma once

#include <cmath>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#include "concepts/NoiseGenType.h"
#include "BoxMullerNoise.h"

namespace glos::clcr 
{

constexpr std::string SAMPLE_RATE_REDUX_DIAL_ID = "SAMPLE_RATE_REDUX";
constexpr std::string SAMPLE_RATE_REDUX_DIAL_TEXT = "Sample Rate Reduction";
constexpr std::string BIT_DEPTH_DIAL_ID = "BIT_DEPTH";
constexpr std::string BIT_DEPTH_DIAL_TEXT = "Bit Depth";
constexpr std::string NOISE_AMOUNT_DIAL_ID = "NOISE_AMOUNT";
constexpr std::string NOISE_AMOUNT_DIAL_TEXT = "Noise Amount";

/**
 * @brief Bit crusher audio effect that reduces the sample rate and bit depth of an audio signal.
 * This effect can be used as a JUCE DSP processor in an audio processing chain.
 * 
 * @tparam NoiseGen Provides noise samples that can be added to the processed signal.
 */
template <typename NoiseGen>
requires NoiseGenType<NoiseGen>
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
     * @brief Initializes with a sample rate reduction of sampleRateRedux and a bit depth of bitDepth with a user
     * provided noise generator.
     * 
     * @param sampleRateRedux The amount the sample rate will be reduced by.
     * @param bitDepth The number of bits used to represent each sample after processing.
     * @param noiseAmount The amount of noise to be added to the processed signal.
     * @param noiseGenerator The noise generator to use for adding noise to the processed signal.
     */
    void init(std::atomic<float>* sampleRateRedux, std::atomic<float>* bitDepth, std::atomic<float>* noiseAmount, NoiseGen noiseGenerator)
    {
        m_sampleRateRedux = sampleRateRedux;
        m_bitDepth = bitDepth;
        m_noiseAmount = noiseAmount;
        m_noiseGenerator = noiseGenerator;
    }

    void init(int numChannels, int sampleRate)
    {
        m_noiseGenerator = NoiseGen(numChannels, sampleRate);
    }

    /**
     * @brief Set the sample rate reduction of the bit crusher.
     * 
     * @param sampleRateRedux
     */
    void setSampleRateRedux(std::atomic<float>* sampleRateRedux) noexcept
    {
        m_sampleRateRedux = sampleRateRedux;
    }

    /**
     * @brief Get the sample rate reduction of the bit crusher.
     * 
     * @return std::atomic<float>* 
     */
    std::atomic<float>* getSampleRateRedux() const noexcept
    {
        return m_sampleRateRedux;
    }

    /**
     * @brief Set the number of bits used to represent each sample after processing.
     * 
     * @param bitDepth 
     */
    void setBitDepth(std::atomic<float>* bitDepth) noexcept
    {
        m_bitDepth = bitDepth;
    }

    /**
     * @brief Get the the bit depth of the bit crusher.
     * 
     * @return std::atomic<float>* 
     */
    std::atomic<float>* getBitDepth() const noexcept
    {
        return m_bitDepth;
    }

    /**
     * @brief Set the amount of noise to be added to the processed signal.
     * 
     * @param noiseAmount 
     */
    void setNoiseAmount(std::atomic<float>* noiseAmount) noexcept
    {
        auto noiseAmountVal = noiseAmount->load();
        jassert(noiseAmountVal >= 0.0f && noiseAmountVal <= 1.0f);
        m_noiseAmount = noiseAmount;
    }

    /**
     * @brief Get the amount of noise to be added to the processed signal.
     * 
     * @return std::atomic<float>* 
     */
    std::atomic<float>* getNoiseAmount() const noexcept
    {
        return m_noiseAmount;
    }

    void prepare (const juce::dsp::ProcessSpec& spec) override
    {

    }
    
    /**
     * @brief 
     * 
     * @param context 
     */
    void process (const juce::dsp::ProcessContextReplacing<float>& context) override
    {
        auto& block = context.getOutputBlock();
        auto numChannels = block.getNumChannels();
        auto numSamples = block.getNumSamples();
        auto noise = m_noiseGenerator.nextNSamples(static_cast<int>(numSamples));

        auto redux = static_cast<int>(m_sampleRateRedux->load());
        for (std::size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* samples = block.getChannelPointer(channel);
            auto* noiseSamples = noise.getWritePointer(static_cast<int>(channel));

            for (std::size_t i = 0; i < numSamples; ++i)
            {
                if (static_cast<int>(i) % redux == 0)
                    m_heldSample = quantize(samples[i]);

                samples[i] = m_heldSample;
            }

            juce::FloatVectorOperations::multiply(noiseSamples, m_noiseAmount->load(), numSamples);
            juce::FloatVectorOperations::add(samples, noiseSamples, numSamples);
        }
    }

    void reset() override
    {

    }

private:
    /**
     * @brief Quantize the sample to the specified bit depth.
     * 
     * @param sample Sample to be quantized.
     * @return float 
     */
    float quantize(float sample) const
    {
        float scale = powf(2.0f, m_bitDepth->load());
        return std::floor(sample * scale) / scale;
    }

    std::atomic<float>* m_sampleRateRedux = nullptr; /**< Amount the sample rate will be reduced by */
    float m_heldSample = 0.0f; /**< Current sample that is held and re-used for m_sampleRateRedux iterations */

    std::atomic<float>* m_bitDepth = nullptr; /**< Number of bits used to represent each sample after processing */
    std::atomic<float>* m_noiseAmount = nullptr; /**< Percentage of signal made up by generated noise */

    NoiseGen m_noiseGenerator = NoiseGen(); /**< Noise generator */
};

using GaussianBitCrusher = BitCrusher<BoxMullerNoise>;

} // namespace glos::clcr