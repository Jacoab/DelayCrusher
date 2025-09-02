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
    auto area = getLocalBounds();
    m_slider.setBounds(area.reduced(5));
}

}