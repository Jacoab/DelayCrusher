#include "PluginEditor.h"

//==============================================================================
CloudCrusherAudioProcessorEditor::CloudCrusherAudioProcessorEditor (CloudCrusherAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);
}

CloudCrusherAudioProcessorEditor::~CloudCrusherAudioProcessorEditor() {}

void CloudCrusherAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("CloudCrusher", getLocalBounds(), juce::Justification::centred, 1);
}

void CloudCrusherAudioProcessorEditor::resized()
{
    // Layout your controls here
}
