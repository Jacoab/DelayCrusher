#include <gtest/gtest.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_core/juce_core.h>

#include "BitCrusher.h"
#include "utils/ConstNoise.h"

TEST(TestBitCrusher, ProcessSample)
{
    auto numChannels = 1;
    auto sampleRate = 44100;
    auto bufferSamples = 16;
    juce::AudioBuffer<float> audioBuffer(numChannels, bufferSamples);
    auto *channelData = audioBuffer.getWritePointer(0);
    for (int i = 0; i < audioBuffer.getNumSamples(); ++i)
    {
        // Alternate between 0.2 and 0.3 for sample rate reduction test
        channelData[i] = i % 2 == 0 ? 0.2f : 0.3f;
    }

    juce::dsp::AudioBlock<float> audioBlock(audioBuffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);

    auto sampleRateRedux = 22050.0f;
    auto bitDepth = 8.0f;

    dlcr::BitCrusher bitCrusher{};
    bitCrusher.setSampleRateRedux(sampleRateRedux);
    bitCrusher.setBitDepth(bitDepth);

    bitCrusher.process(context);

    int holdInterval = static_cast<int>(sampleRate / sampleRateRedux + 0.5f);

    auto& block = context.getOutputBlock();
    auto* processedChannelData = block.getChannelPointer(0);

    float quantized = std::floor(0.2f * 256.0f) / 256.0f;
    for (int i = 0; i < block.getNumSamples(); ++i)
    {
        if (i % holdInterval == 0)
            quantized = std::floor(channelData[i] * 256.0f) / 256.0f;
        EXPECT_NEAR(processedChannelData[i], quantized, 0.0001f);
    }
}