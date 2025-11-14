#include <cmath>

#include <gtest/gtest.h>
#include <juce_dsp/juce_dsp.h>

#include "BoxMullerNoise.h"

TEST(TestBoxMullerNoise, KolmogorovSmirnovTest)
{
    auto numChannels = 1;
    auto sampleRate = 10000.0;
    auto numSamples = static_cast<uint32_t>(sampleRate);

    // Create audio buffer to fill with noise samples
    juce::AudioBuffer<float> audioBuffer(numChannels, numSamples);
    audioBuffer.clear();

    // Prepare the processor
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.numChannels = static_cast<uint32_t>(numChannels);
    spec.maximumBlockSize = numSamples;

    std::atomic<float> noiseAmount(1.0f);
    glos::clcr::BoxMullerNoise noiseGenerator;
    noiseGenerator.setNoiseAmountParam(&noiseAmount);
    noiseGenerator.prepare(spec);

    // Process the audio block to generate noise
    juce::dsp::AudioBlock<float> audioBlock(audioBuffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);
    noiseGenerator.process(context);

    // Get the generated noise samples
    auto* channelData = audioBlock.getChannelPointer(0);

    // Sort samples for K-S test
    std::sort(channelData, channelData + numSamples);

    auto dMax = 0.0f;
    for (int i = 0; i < numSamples; ++i)
    {
        auto cdf = 0.5f * std::erfc(-channelData[i] * static_cast<float>(M_SQRT1_2));
        auto expectedCDF = static_cast<float>(i + 1) / numSamples;
        auto d = std::max(std::abs(expectedCDF - cdf), std::abs(cdf - static_cast<float>(i) / numSamples));
        
        if (d > dMax) dMax = d;
    }

    // Kolmogorov-Smirnov critical value for alpha = 0.05
    float ksCritical = 1.36f / std::sqrt(static_cast<float>(numSamples));

    ASSERT_TRUE(dMax < ksCritical);
}