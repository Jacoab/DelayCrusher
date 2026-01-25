#include "PluginEditor.h"

namespace glos::clcr
{

CloudCrusherAudioProcessorEditor::CloudCrusherAudioProcessorEditor (CloudCrusherAudioProcessor& p) : 
    AudioProcessorEditor (&p), 
    m_audioProcessor (p),
    m_bitCrusherUI(m_audioProcessor.getAPVTS()),
    m_delayUI(m_audioProcessor.getAPVTS())
{
    setSize (600, 400);

    addAndMakeVisible(m_bitCrusherUI);
    addAndMakeVisible(m_delayUI);
}

CloudCrusherAudioProcessorEditor::~CloudCrusherAudioProcessorEditor() {}

void CloudCrusherAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(static_cast<uint32_t>(ColorPalette::Background)));
    g.setColour (juce::Colour(static_cast<uint32_t>(ColorPalette::PrimaryVariant)));
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
    fb.alignItems = juce::FlexBox::AlignItems::center;
    
    fb.items.add(
    juce::FlexItem(m_bitCrusherUI)
        .withWidth(320)
        .withHeight(200)
        .withMargin(10)
    );
    fb.items.add(juce::FlexItem(m_delayUI)
        .withWidth(230)
        .withHeight(200)
        .withMargin(10)
    );

    fb.performLayout(getLocalBounds());
}

}