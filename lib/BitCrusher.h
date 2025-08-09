#pragma once

#include <cmath>

#include <juce_audio_processors/juce_audio_processors.h>

#include "concepts/NoiseGenType.h"
#include "BoxMullerNoise.h"

namespace glos::clcr
{

/**
 * @brief Bit crusher audio effect that reduces the sample rate and bit depth of an audio signal.
 * 
 * @tparam NoiseGen Provides noise samples that can be added to the processed signal.
 */
template <typename NoiseGen>
requires NoiseGenType<NoiseGen>
class BitCrusher
{
public:
    /**
     * @brief Default constructor for BitCrusher.
     * 
     * Initializes with a sample rate reduction of 100 and a bit depth of 8.
     */
    BitCrusher() :
        m_sampleRateRedux(0),
        m_sampleRateCounter(0),
        m_bitDepth(32),
        m_heldSample(0.0f),
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
        m_sampleRateCounter(0),
        m_bitDepth(bitDepth),
        m_heldSample(0.0f),
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
        m_sampleRateCounter(0),
        m_bitDepth(bitDepth),
        m_heldSample(0.0f),
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

    /**
     * @brief Process a single sample by quantizing the sample to the specified bit depth, reducing the sample rate,
     * and dithering noise to the quantized sample.
     * 
     * @param sample Sample to be processed.
     * @return float 
     */
    float process(float sample) noexcept
    {
        if (--m_sampleRateCounter <= 0)
        {
            m_sampleRateCounter = m_sampleRateRedux;
            m_heldSample = quantize(sample);
        }

        auto noise = m_noiseGenerator.nextSample();
        return m_heldSample + noise * m_noiseAmount;
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
    int m_sampleRateCounter; /**< The number of iterations the current sample should be held and re-used */
    float m_heldSample; /**< Current sample that is held and re-used for m_sampleRateCounter iterations */

    std::atomic_int m_bitDepth; /**< Number of bits used to represent each sample after processing */
    std::atomic<float> m_noiseAmount; /**< Percentage of signal made up by generated noise */

    NoiseGen m_noiseGenerator; /**< Noise generator */
};

} // namespace glos::clcr