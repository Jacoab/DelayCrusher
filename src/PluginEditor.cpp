#include "PluginEditor.h"

namespace glos::clcr
{

CloudCrusherAudioProcessorEditor::CloudCrusherAudioProcessorEditor (CloudCrusherAudioProcessor& p) : 
    AudioProcessorEditor (&p), 
    m_audioProcessor (p),
    m_bitCrusherUI(m_audioProcessor.getAPVTS()),
    m_delayUI(m_audioProcessor.getAPVTS())
{
    setSize (800, 600);

    addAndMakeVisible(m_bitCrusherUI);
    addAndMakeVisible(m_delayUI);
}

CloudCrusherAudioProcessorEditor::~CloudCrusherAudioProcessorEditor() {}

void CloudCrusherAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    auto topArea = getLocalBounds();
    g.drawFittedText ("Cloud Crusher", topArea, juce::Justification::centredTop, 1);
}

void CloudCrusherAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto flexBasis = static_cast<float>(getWidth());

    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    
    fb.items.add(juce::FlexItem(m_bitCrusherUI).withFlex(0, 1, flexBasis));
    fb.items.add(juce::FlexItem(m_delayUI).withFlex(0, 1, flexBasis));

    fb.performLayout(getLocalBounds());
}

}