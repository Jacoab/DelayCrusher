#include <gtest/gtest.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_core/juce_core.h>

#include "BitCrusher.h"
#include "utils/ConstNoise.h"

TEST(TestBitCrusher, ProcessSample)
{
    auto testNoise = ConstNoise(0.01f);

    auto numChannels = 1;
    auto sampleRate = 44100;
    juce::AudioBuffer<float> audioBuffer(numChannels, sampleRate);
    auto *channelData = audioBuffer.getWritePointer(0);
    for (int i = 0; i < audioBuffer.getNumSamples(); ++i)
    {
        // We want to test the sample rate reduction so we alternate between 0.2 and 0.3
        // so that with a sample rate reduction of 2, every even sample will be used and our
        // bit crusher will work with a buffer of values all equal to 0.2.
        channelData[i] = i % 2 == 0 ? 0.2f : 0.3f;
    }

    juce::dsp::AudioBlock<float> audioBlock(audioBuffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);

    std::atomic<float> sampleRateRedux = 2;
    std::atomic<float> bitDepth = 8;
    std::atomic<float> noiseAmount = 0.5f;
    auto* sampleRateReduxPtr = &sampleRateRedux;
    auto* bitDepthPtr = &bitDepth;
    auto* noiseAmountPtr = &noiseAmount;
    glos::clcr::BitCrusher<ConstNoise> bitCrusher{};
    bitCrusher.init(sampleRateReduxPtr, bitDepthPtr, noiseAmountPtr, testNoise);

    bitCrusher.process(context);

    auto expectedProcessedSample = 0.20421875f;
    auto& block = context.getOutputBlock();
    auto* processedChannelData = block.getChannelPointer(0);
    auto sampleIsCorrect = true;
    for (std::size_t i = 0; i < block.getNumSamples(); ++i)
    {
        if (std::abs(processedChannelData[i] - expectedProcessedSample) > 0.00001f)
        {
            sampleIsCorrect = false;
            break;
        }
    }
    
    ASSERT_TRUE(sampleIsCorrect);
}