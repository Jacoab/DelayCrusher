#include <gtest/gtest.h>

#include "Delay.h"

TEST(TestDelay, ProcessDelay)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 44100.0;

    std::atomic<float> delayTime(0.5f); // 0.5 seconds delay
    auto delayTimeInSamples = static_cast<int>(delayTime.load() * spec.sampleRate);

    // make room for some samples after the delay
    spec.maximumBlockSize = delayTimeInSamples + 10; // Extra samples to accommodate delay
    spec.numChannels = 2;

    glos::clcr::Delay delay;
    
    delay.setDelayTime(&delayTime);
    delay.prepare(spec);

    // allocate buffer using maximumBlockSize so there are samples after the delay
    juce::AudioBuffer<float> buffer(static_cast<int>(spec.numChannels), static_cast<int>(spec.maximumBlockSize));
    buffer.clear();

    auto numChannels = buffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();

    // Fill buffer: first 'delayTimeInSamples' samples are zero, remaining samples are 0.5f
    for (int ch = 0; ch < numChannels; ++ch)
    {
        auto writeBuffer = buffer.getWritePointer(ch);
        for (int i = 0; i < delayTimeInSamples; ++i)
            writeBuffer[i] = 0.0f;
        for (int i = delayTimeInSamples; i < numSamples; ++i)
            writeBuffer[i] = 0.5f;
    }

    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);

    delay.process(context);

    // The first sample that should reflect the delayed input is at index 'delayTimeInSamples'
    auto* channelData = context.getOutputBlock().getChannelPointer(0);
    for (int i = delayTimeInSamples; i < spec.maximumBlockSize; ++i)
    {
        EXPECT_FLOAT_EQ(channelData[i], 0.5f);
    }
    EXPECT_FLOAT_EQ(channelData[delayTimeInSamples], 0.5f);
}
