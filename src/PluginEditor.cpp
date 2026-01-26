#include "PluginEditor.h"

namespace dlcr
{

DelayCrusherAudioProcessorEditor::DelayCrusherAudioProcessorEditor (DelayCrusherAudioProcessor& p) : 
    AudioProcessorEditor (&p), 
    m_audioProcessor (p),
    m_bitCrusherUI(m_audioProcessor.getAPVTS()),
    m_delayUI(m_audioProcessor.getAPVTS())
{
    setSize (600, 400);

    addAndMakeVisible(m_bitCrusherUI);
    addAndMakeVisible(m_delayUI);
}

DelayCrusherAudioProcessorEditor::~DelayCrusherAudioProcessorEditor() {}

void DelayCrusherAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto primaryVariantColor = getColorFromPalette(ColorPalette::PrimaryVariant);
    auto backgroundColor = getColorFromPalette(ColorPalette::Background);

    g.fillAll(backgroundColor);
    g.setColour (primaryVariantColor);
    g.setFont (15.0f);

    auto topArea = getLocalBounds();
    g.drawFittedText ("Delay Crusher", topArea, juce::Justification::centredTop, 1);
}

void DelayCrusherAudioProcessorEditor::resized()
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