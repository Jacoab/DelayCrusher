#include "components/DelayUI.h"

namespace glos::clcr
{

DelayUI::DelayUI(juce::AudioProcessorValueTreeState& apvts) :
    m_delayTimeDial(Delay::DELAY_TIME_DIAL_TEXT, apvts, Delay::DELAY_TIME_DIAL_ID),
    m_delayDryWetDial(Delay::DRY_WET_DIAL_TEXT, apvts, Delay:: DRY_WET_DIAL_ID)
{
    addAndMakeVisible(m_delayTimeDial);
    addAndMakeVisible(m_delayDryWetDial);
}


void DelayUI::resized()
{
    auto bounds = getLocalBounds();
    auto flexBasis = static_cast<float>(getWidth() / 3);
    
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    
    fb.items.add(juce::FlexItem(m_delayTimeDial).withFlex(1).withMargin(10));
    fb.items.add(juce::FlexItem(m_delayDryWetDial).withFlex(1).withMargin(10));

    fb.performLayout(bounds);
}

void DelayUI::paint(juce::Graphics& g)
{
    constexpr auto cornerRadius = 6.0f;
    constexpr auto stroke = 1.5f;

    const std::string title = "Delay";

    g.setFont (14.0f);
    const auto titleHeight = juce::roundToInt (g.getCurrentFont().getHeight());

    auto bounds = getLocalBounds().toFloat();

    auto box = bounds;
    box.setY (box.getY() + titleHeight * 0.5f);
    box.setHeight (box.getHeight() - titleHeight * 0.5f);

    g.setColour (juce::Colours::white);
    g.drawRoundedRectangle (box.reduced (stroke * 0.5f),
                            cornerRadius,
                            stroke);

    const auto textWidth = g.getCurrentFont().getStringWidth(title);
    const auto textX = static_cast<int> (bounds.getCentreX() - textWidth * 0.5f);
    const auto textY = 0;

    g.setColour (juce::Colours::black);
    g.fillRect (textX - 6,
                textY,
                textWidth + 12,
                titleHeight);

    g.setColour (juce::Colours::white);
    g.drawText (title,
                textX,
                textY,
                textWidth,
                titleHeight,
                juce::Justification::centred,
                false);
}

}