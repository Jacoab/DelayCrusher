#include <string_view>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "juce_audio_processors/juce_audio_processors.h"

#include "ProcessorParam.h"
#include "utils/MockAudioProcessor.h"

constexpr char paramID[] = "TEST_PARAM";
constexpr char paramName[] = "Test Parameter";

TEST(TestProcessorParam, ParameterChangedUpdatesValue)
{
    glos::clcr::ProcessorParam<paramID, paramName, float> processorParam(0.0f);

    float newValue = 5.0f;
    processorParam.parameterChanged(paramID, newValue);

    EXPECT_FLOAT_EQ(processorParam.load(), newValue);
}

TEST(TestProcessorParam, APVTSChangeUpdatesValue)
{
    juce::ScopedJuceInitialiser_GUI juceInit;

    // APVTS needs an audio processor instance so we will use a mock
    MockAudioProcessor mockProcessor;

    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        paramID,
        paramName,
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        0.0f
    ));

    juce::AudioProcessorValueTreeState apvts(mockProcessor, nullptr, "Params", std::move(layout));

    // ProcessorParam instance and registration as listener
    glos::clcr::ProcessorParam<paramID, paramName, float> processorParam(0.0f);
    apvts.addParameterListener(paramID, &processorParam);

    // Change the parameter via APVTS (use convertTo0to1 to get normalized value)
    auto* param = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(paramID));
    ASSERT_NE(param, nullptr);
    float newValue = 10.0f;
    param->setValueNotifyingHost(param->convertTo0to1(newValue));

    // The listener's parameterChanged should have been called synchronously
    EXPECT_FLOAT_EQ(processorParam.load(), newValue);

    // Clean up listener registration (optional)
    apvts.removeParameterListener(paramID, &processorParam);
}
