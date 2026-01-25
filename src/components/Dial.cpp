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
    m_label.setColour(juce::Label::textColourId, juce::Colour(static_cast<int>(ColorPalette::PrimaryVariant)));

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

void Dial::LookAndFeel::drawRotarySlider(
    juce::Graphics& g,
    int x,
    int y,
    int width,
    int height,
    float sliderPos,
    const float rotaryStartAngle,
    const float rotaryEndAngle,
    juce::Slider&
)
{
    auto radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
    auto centreX = (float) x + (float) width * 0.5f;
    auto centreY = (float) y + (float) height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // fill
    g.setColour(juce::Colour(static_cast<int>(ColorPalette::Primary)));
    g.fillEllipse (rx, ry, rw, rw);
    
    // outline
    g.setColour(juce::Colour(static_cast<int>(ColorPalette::Secondary)));
    g.drawEllipse(rx, ry, rw, rw, 1.0f);

    juce::Path p;
    auto pointerLength = radius * 0.7f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation (angle).translated (centreX, centreY));

    g.setColour(juce::Colour(static_cast<int>(ColorPalette::Secondary)));
    g.fillPath (p);
}

juce::Label* Dial::LookAndFeel::createSliderTextBox (juce::Slider& slider)
{
    auto* label = juce::LookAndFeel_V4::createSliderTextBox(slider);

    label->setColour(
        juce::Label::textColourId,
        juce::Colour(static_cast<int>(ColorPalette::PrimaryVariant))
    );

    label->setColour(
        juce::Label::textWhenEditingColourId,
        juce::Colour(static_cast<int>(ColorPalette::PrimaryVariant))
    );

    label->setColour(
        juce::Label::backgroundColourId,
        juce::Colour(static_cast<int>(ColorPalette::Background))
    );

    label->setColour(
        juce::Label::outlineColourId,
        juce::Colour(static_cast<int>(ColorPalette::PrimaryVariant))
    );

    label->setJustificationType(juce::Justification::centred);
    label->setBorderSize(juce::BorderSize<int>(0));

    return label;
}

}