#pragma once

#include <cmath>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#include "concepts/NoiseGenType.h"
#include "BoxMullerNoise.h"

namespace glos::clcr 
{

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
     * @brief Default constructor for BitCrusher.
     * 
     * Initializes with a sample rate reduction of 100 and a bit depth of 8.
     */
    BitCrusher() :
        m_sampleRateRedux(0),
        m_heldSample(0.0f),
        m_bitDepth(32),
        m_noiseGenerator(NoiseGen())
    {
    }

    /**
     * @brief Default constructor for BitCrusher.
     * 
     * @param sampleRateRedux The amount the sample rate will be reduced by.
     * @param bitDepth The number of bits used to represent each sample after processing.
     * Initializes with a sample rate reduction of sampleRateRedux and a bit depth of bitDepth.
     */
    BitCrusher(int sampleRateRedux, int bitDepth) : 
        m_sampleRateRedux(sampleRateRedux),
        m_heldSample(0.0f),
        m_bitDepth(bitDepth),
        m_noiseGenerator(NoiseGen())
    {
    }

    /**
     * @brief Default constructor for BitCrusher.
     * 
     * @param sampleRateRedux The amount the sample rate will be reduced by.
     * @param bitDepth The number of bits used to represent each sample after processing.
     * Initializes with a sample rate reduction of sampleRateRedux and a bit depth of bitDepth.
     */
    BitCrusher(int sampleRateRedux, int bitDepth, NoiseGen noiseGenerator) : 
        m_sampleRateRedux(sampleRateRedux),
        m_heldSample(0.0f),
        m_bitDepth(bitDepth),
        m_noiseGenerator(noiseGenerator)
    {
    }

    /**
     * @brief Set the sample rate reduction of the bit crusher.
     * 
     * @param sampleRateRedux
     */
    void setSampleRateRedux(int sampleRateRedux) noexcept
    {
        m_sampleRateRedux = sampleRateRedux;
    }

    /**
     * @brief Set the number of bits used to represent each sample after processing.
     * 
     * @param bitDepth 
     */
    void setBitDepth(int bitDepth) noexcept
    {
        m_bitDepth = bitDepth;
    }

    /**
     * @brief Set the amount of noise to be added to the processed signal.
     * 
     * @param noiseAmount 
     */
    void setNoiseAmount(float noiseAmount) noexcept
    {
        jassert(noiseAmount >= 0.0f && noiseAmount <= 1.0f);
        m_noiseAmount = noiseAmount;
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

        for (std::size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* samples = block.getChannelPointer(channel);
            for (std::size_t i = 0; i < numSamples; ++i)
            {
                // Reuse every sample that is a multiple of m_sampleRateRedux 
                if (i % m_sampleRateRedux == 0)
                    m_heldSample = quantize(samples[i]);

                samples[i] = m_heldSample;
            }
            
            auto* noiseSamples = noise.getWritePointer(channel);
            juce::FloatVectorOperations::multiply(noiseSamples, m_noiseAmount.load(), numSamples);
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
        float scale = powf(2.0f, m_bitDepth);
        return std::floor(sample * scale) / scale;
    }

    std::atomic_int m_sampleRateRedux; /**< Amount the sample rate will be reduced by */
    float m_heldSample; /**< Current sample that is held and re-used for m_sampleRateRedux iterations */

    std::atomic_int m_bitDepth; /**< Number of bits used to represent each sample after processing */
    std::atomic<float> m_noiseAmount; /**< Percentage of signal made up by generated noise */

    NoiseGen m_noiseGenerator; /**< Noise generator */
};

using GaussianBitCrusher = BitCrusher<BoxMullerNoise>;

} // namespace glos::clcr