#include <gtest/gtest.h>

#include "PluginProcessor.h"
#include "BitCrusher.h"


TEST(TestPluginProcessor, BitCrusherParameterSync)
{
    juce::ScopedJuceInitialiser_GUI juceInit;
    dlcr::DelayCrusherAudioProcessor processor{};
    auto& apvts = processor.getAPVTS();

    processor.prepareToPlay(44100.0, 512);

    const auto& bitCrusher = processor.getBitCrusher();
    const auto& noiseGenerator = processor.getNoiseGenerator();
    const auto& delay = processor.getDelay();

    auto* sampleRateReduxParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(dlcr::BitCrusher::SAMPLE_RATE_REDUX_DIAL_ID));
    auto* bitDepthParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(dlcr::BitCrusher::BIT_DEPTH_DIAL_ID));
    auto* noiseAmountParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(dlcr::BoxMullerNoise::NOISE_AMOUNT_DIAL_ID));
    auto* delayTimeParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(dlcr::Delay::DELAY_TIME_DIAL_ID));
    auto* delayDryWetParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(dlcr::Delay::DRY_WET_DIAL_ID));

    ASSERT_NE(sampleRateReduxParam, nullptr);
    ASSERT_NE(bitDepthParam, nullptr);
    ASSERT_NE(noiseAmountParam, nullptr);

    auto sampleRateRedux = 4.0f;
    auto bitDepth = 12.0f;
    auto noiseAmount = 0.75f;
    auto delayTime = 250.0f;
    auto delayDryWet = 0.5f;
    
    sampleRateReduxParam->setValueNotifyingHost(sampleRateReduxParam->convertTo0to1(sampleRateRedux));
    bitDepthParam->setValueNotifyingHost(bitDepthParam->convertTo0to1(bitDepth));
    noiseAmountParam->setValueNotifyingHost(noiseAmountParam->convertTo0to1(noiseAmount));
    delayTimeParam->setValueNotifyingHost(delayTimeParam->convertTo0to1(delayTime));
    delayDryWetParam->setValueNotifyingHost(delayDryWetParam->convertTo0to1(delayDryWet));

    EXPECT_FLOAT_EQ(bitCrusher.getSampleRateRedux(), sampleRateRedux);
    EXPECT_FLOAT_EQ(bitCrusher.getBitDepth(), bitDepth);
    EXPECT_FLOAT_EQ(noiseGenerator.getNoiseAmount(), noiseAmount);
    EXPECT_FLOAT_EQ(delay.getDelayTime(), delayTime);
    EXPECT_FLOAT_EQ(delay.getDryWet(), delayDryWet);
}


TEST(TestPluginProcessor, ProcessBlockUsesUpdatedParameters)
{
    juce::ScopedJuceInitialiser_GUI juceInit;
    dlcr::DelayCrusherAudioProcessor processor{};
    processor.prepareToPlay(44100.0, 512);

    auto& apvts = processor.getAPVTS();
    auto* sampleRateReduxParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(dlcr::BitCrusher::SAMPLE_RATE_REDUX_DIAL_ID));
    auto* bitDepthParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(dlcr::BitCrusher::BIT_DEPTH_DIAL_ID));
    auto* noiseAmountParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(dlcr::BoxMullerNoise::NOISE_AMOUNT_DIAL_ID));
    auto* delayTimeParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(dlcr::Delay::DELAY_TIME_DIAL_ID));
    auto* delayDryWetParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(dlcr::Delay::DRY_WET_DIAL_ID));

    ASSERT_NE(sampleRateReduxParam, nullptr);
    ASSERT_NE(bitDepthParam, nullptr);
    ASSERT_NE(noiseAmountParam, nullptr);

    sampleRateReduxParam->setValueNotifyingHost(sampleRateReduxParam->convertTo0to1(2.0f));
    bitDepthParam->setValueNotifyingHost(bitDepthParam->convertTo0to1(8.0f));
    noiseAmountParam->setValueNotifyingHost(noiseAmountParam->convertTo0to1(0.5f));
    delayTimeParam->setValueNotifyingHost(delayTimeParam->convertTo0to1(1000.0f));
    delayDryWetParam->setValueNotifyingHost(delayDryWetParam->convertTo0to1(0.5f));

    juce::AudioBuffer<float> buffer(2, 512);
    juce::MidiBuffer midi;
    buffer.clear();

    // Fill buffer with a known value
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            buffer.setSample(ch, i, 0.2f);

    processor.processBlock(buffer, midi);

    // After processing, buffer should be modified (not all zeros)
    float sum = 0.0f;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            sum += buffer.getSample(ch, i);
    }

    EXPECT_NE(sum, 0.0f);
}