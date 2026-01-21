#include "components/BitCrusherUI.h"

namespace glos::clcr
{

BitCrusherUI::BitCrusherUI(juce::AudioProcessorValueTreeState& apvts) :
    m_sampleRateReduxDial(BitCrusher::SAMPLE_RATE_REDUX_DIAL_TEXT, apvts,  BitCrusher::SAMPLE_RATE_REDUX_DIAL_ID),
    m_bitDepthDial(BitCrusher::BIT_DEPTH_DIAL_TEXT, apvts, BitCrusher::BIT_DEPTH_DIAL_ID),
    m_noiseAmountDial(BoxMullerNoise::NOISE_AMOUNT_DIAL_TEXT, apvts, BoxMullerNoise::NOISE_AMOUNT_DIAL_ID)
{
    addAndMakeVisible(m_sampleRateReduxDial);
    addAndMakeVisible(m_bitDepthDial);
    addAndMakeVisible(m_noiseAmountDial);
}


void BitCrusherUI::resized()
{
    auto bounds = getLocalBounds();
    auto flexBasis = static_cast<float>(getWidth() / 3);
    
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    
    fb.items.add(juce::FlexItem(m_sampleRateReduxDial).withFlex(0, 1, flexBasis));
    fb.items.add(juce::FlexItem(m_bitDepthDial).withFlex(0, 1 , flexBasis));
    fb.items.add(juce::FlexItem(m_noiseAmountDial).withFlex(0, 1, flexBasis));

    fb.performLayout(bounds);
}

}