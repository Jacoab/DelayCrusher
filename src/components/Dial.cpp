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
    m_slider.setLookAndFeel(&m_lookAndFeel);

    m_label.setJustificationType(juce::Justification::centredTop);
    m_label.setBorderSize(juce::BorderSize<int>(0));

    m_attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, paramID, m_slider);

    addAndMakeVisible(m_slider);
    addAndMakeVisible(m_label);
}

Dial::~Dial()
{
    m_slider.setLookAndFeel(nullptr);
}

void Dial::resized()
{
    juce::FlexBox fb;
    fb.flexDirection  = juce::FlexBox::Direction::column;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;

    fb.items.add(
        juce::FlexItem(m_slider)
            .withFlex(0)
            .withMinHeight(120)
    );

    fb.items.add(
        juce::FlexItem(m_label)
            .withHeight(20)
            .withMargin({ 4, 0, 0, 0 })
    );

    fb.performLayout(getLocalBounds());
}

juce::Slider::SliderLayout Dial::LookAndFeel::getSliderLayout(juce::Slider& slider)
{
    auto layout = juce::LookAndFeel_V4::getSliderLayout(slider);
    
    if (slider.getSliderStyle() == juce::Slider::Rotary)
    {
        auto bounds = slider.getLocalBounds();
        int textBoxHeight = 20;
        int dialSize = 80;
        int totalHeight = dialSize + textBoxHeight;
        int startY = bounds.getY() + (bounds.getHeight() - totalHeight) / 2;
        int dialX = bounds.getX() + (bounds.getWidth() - dialSize) / 2;

        layout.sliderBounds = juce::Rectangle<int>(dialX, startY, dialSize, dialSize);
        layout.textBoxBounds = juce::Rectangle<int>(
            dialX,
            startY + dialSize,
            dialSize,
            textBoxHeight
        );
    }
    
    return layout;
}

}