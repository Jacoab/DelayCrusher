#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "components/Dial.h"
#include "processors/BitCrusher.h"
#include "processors/Delay.h"
#include "PluginProcessor.h"

namespace glos::clcr
{

class CloudCrusherAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit CloudCrusherAudioProcessorEditor (CloudCrusherAudioProcessor&);
    ~CloudCrusherAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CloudCrusherAudioProcessor& m_audioProcessor;
    Dial m_sampleRateReduxDial;
    Dial m_bitDepthDial;
    Dial m_noiseAmountDial;

    Dial m_delayTimeDial;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CloudCrusherAudioProcessorEditor)
};

}