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
    
    fb.items.add(juce::FlexItem(m_delayTimeDial).withFlex(0, 1, flexBasis));
    fb.items.add(juce::FlexItem(m_delayDryWetDial).withFlex(0, 1 , flexBasis));

    fb.performLayout(bounds);
}

}