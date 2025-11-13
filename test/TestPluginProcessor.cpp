#include <gtest/gtest.h>

#include "PluginProcessor.h"
#include "BitCrusher.h"


TEST(TestPluginProcessor, BitCrusherParameterSync)
{
    juce::ScopedJuceInitialiser_GUI juceInit;
    glos::clcr::CloudCrusherAudioProcessor processor{};
    auto& apvts = processor.getAPVTS();

    processor.prepareToPlay(44100.0, 512);

    const auto& bitCrusher = processor.getBitCrusher();
    const auto& noiseGenerator = processor.getNoiseGenerator();
    const auto& delay = processor.getDelay();

    auto* sampleRateReduxParam = apvts.getRawParameterValue(glos::clcr::SAMPLE_RATE_REDUX_DIAL_ID);
    auto* bitDepthParam = apvts.getRawParameterValue(glos::clcr::BIT_DEPTH_DIAL_ID);
    auto* noiseAmountParam = apvts.getRawParameterValue(glos::clcr::NOISE_AMOUNT_DIAL_ID);
    auto* delayTimeParam = apvts.getRawParameterValue(glos::clcr::DELAY_TIME_DIAL_ID);
    auto* delayDryWetParam = apvts.getRawParameterValue(glos::clcr::DELAY_DRY_WET_DIAL_ID);

    auto sampleRateRedux = 4.0f;
    auto bitDepth = 12.0f;
    auto noiseAmount = 0.75f;
    auto delayTime = 250.0f;
    auto delayDryWet = 0.5f;
    
    sampleRateReduxParam->store(sampleRateRedux);
    bitDepthParam->store(bitDepth);
    noiseAmountParam->store(noiseAmount);
    delayTimeParam->store(delayTime);
    delayDryWetParam->store(delayDryWet);

    ASSERT_EQ(bitCrusher.getSampleRateRedux(), sampleRateReduxParam);
    ASSERT_EQ(bitCrusher.getBitDepth(), bitDepthParam);
    ASSERT_EQ(noiseGenerator.getNoiseAmount(), noiseAmountParam);

    EXPECT_FLOAT_EQ(bitCrusher.getSampleRateRedux()->load(), sampleRateRedux);
    EXPECT_FLOAT_EQ(bitCrusher.getBitDepth()->load(), bitDepth);
    EXPECT_FLOAT_EQ(noiseGenerator.getNoiseAmount()->load(), noiseAmount);
    EXPECT_FLOAT_EQ(delay.getDelayTime(), delayTime);
    EXPECT_FLOAT_EQ(delay.getDryWet(), delayDryWet);
}


TEST(TestPluginProcessor, ProcessBlockUsesUpdatedParameters)
{
    juce::ScopedJuceInitialiser_GUI juceInit;
    glos::clcr::CloudCrusherAudioProcessor processor{};
    processor.prepareToPlay(44100.0, 512);

    auto& apvts = processor.getAPVTS();
    auto* sampleRateReduxParam = apvts.getRawParameterValue(glos::clcr::SAMPLE_RATE_REDUX_DIAL_ID);
    auto* bitDepthParam = apvts.getRawParameterValue(glos::clcr::BIT_DEPTH_DIAL_ID);
    auto* noiseAmountParam = apvts.getRawParameterValue(glos::clcr::NOISE_AMOUNT_DIAL_ID);
    auto* delayTimeParam = apvts.getRawParameterValue(glos::clcr::DELAY_TIME_DIAL_ID);
    auto* delayDryWetParam = apvts.getRawParameterValue(glos::clcr::DELAY_DRY_WET_DIAL_ID);

    sampleRateReduxParam->store(2.0f);
    bitDepthParam->store(8.0f);
    noiseAmountParam->store(0.5f);
    delayTimeParam->store(1000.0f);
    delayDryWetParam->store(0.5f);

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