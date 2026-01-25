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

void BitCrusherUI::paint(juce::Graphics& g)
{
    constexpr auto cornerRadius = 6.0f;
    constexpr auto stroke = 1.5f;

    const std::string title = "Bit Crusher";

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

void BitCrusherUI::resized()
{
    auto bounds = getLocalBounds();
    auto flexBasis = static_cast<float>(getWidth() / 3);
    
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    
    fb.items.add(juce::FlexItem(m_sampleRateReduxDial).withFlex(1).withMargin(10));
    fb.items.add(juce::FlexItem(m_bitDepthDial).withFlex(1).withMargin(10));
    fb.items.add(juce::FlexItem(m_noiseAmountDial).withFlex(1).withMargin(10));

    fb.performLayout(bounds);
}

}