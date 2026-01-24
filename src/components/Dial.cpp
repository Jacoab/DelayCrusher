#include "Dial.h"

namespace glos::clcr
{

Dial::Dial(const juce::String& name,
           juce::AudioProcessorValueTreeState& apvts,
           const juce::String& paramID) :
    m_label(name, name),
    m_attachment(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, m_slider))
{
    m_slider.setSliderStyle(juce::Slider::Rotary);
    m_slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    m_label.setJustificationType(juce::Justification::centred);

    m_attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, paramID, m_slider);

    addAndMakeVisible(m_slider);
    addAndMakeVisible(m_label);
}

void Dial::resized()
{
    auto bounds = getLocalBounds();
    auto flexBasis = static_cast<float>(getWidth());

    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::column;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    
    fb.items.add(juce::FlexItem(m_slider).withFlex(3));
    fb.items.add(juce::FlexItem(m_label).withFlex(1));

    fb.performLayout(bounds);
}

}