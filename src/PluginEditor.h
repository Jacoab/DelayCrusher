#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class CloudCrusherAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit CloudCrusherAudioProcessorEditor (CloudCrusherAudioProcessor&);
    ~CloudCrusherAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CloudCrusherAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CloudCrusherAudioProcessorEditor)
};
