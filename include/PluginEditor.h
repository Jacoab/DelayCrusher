#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "components/Dial.h"
#include "PluginProcessor.h"
#include "BitCrusher.h"

//==============================================================================
class CloudCrusherAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit CloudCrusherAudioProcessorEditor (CloudCrusherAudioProcessor&);
    ~CloudCrusherAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CloudCrusherAudioProcessor& m_audioProcessor;
    glos::clcr::Dial m_sampleRateReduxDial;
    glos::clcr::Dial m_bitDepthDial;
    glos::clcr::Dial m_noiseAmountDial;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CloudCrusherAudioProcessorEditor)
};
