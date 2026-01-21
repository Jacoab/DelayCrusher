#include "PluginEditor.h"

namespace glos::clcr
{

CloudCrusherAudioProcessorEditor::CloudCrusherAudioProcessorEditor (CloudCrusherAudioProcessor& p) : 
    AudioProcessorEditor (&p), 
    m_audioProcessor (p),
    m_sampleRateReduxDial(BitCrusher::SAMPLE_RATE_REDUX_DIAL_TEXT, m_audioProcessor.getAPVTS(), BitCrusher::SAMPLE_RATE_REDUX_DIAL_ID),
    m_bitDepthDial(BitCrusher::BIT_DEPTH_DIAL_TEXT, m_audioProcessor.getAPVTS(), BitCrusher::BIT_DEPTH_DIAL_ID),
    m_noiseAmountDial(BoxMullerNoise::NOISE_AMOUNT_DIAL_ID, m_audioProcessor.getAPVTS(), BoxMullerNoise::NOISE_AMOUNT_DIAL_ID),
    m_delayTimeDial(Delay::DELAY_TIME_DIAL_TEXT, m_audioProcessor.getAPVTS(), Delay::DELAY_TIME_DIAL_ID),
    m_delayDryWetDial(Delay::DRY_WET_DIAL_TEXT, m_audioProcessor.getAPVTS(),Delay:: DRY_WET_DIAL_ID)
{
    setSize (400, 300);

    addAndMakeVisible(m_sampleRateReduxDial);
    addAndMakeVisible(m_bitDepthDial);
    addAndMakeVisible(m_noiseAmountDial);

    addAndMakeVisible(m_delayTimeDial);
    addAndMakeVisible(m_delayDryWetDial);
}

CloudCrusherAudioProcessorEditor::~CloudCrusherAudioProcessorEditor() {}

void CloudCrusherAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    auto topArea = getLocalBounds();
    g.drawFittedText ("Cloud Crusher", topArea, juce::Justification::centredTop, 1);
}

void CloudCrusherAudioProcessorEditor::resized()
{

    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    auto flexBasis = static_cast<float>(getHeight() / 3.0f);
    
    fb.items.add(juce::FlexItem(m_sampleRateReduxDial).withFlex(0, 1, flexBasis));
    fb.items.add(juce::FlexItem(m_bitDepthDial).withFlex(0, 1, flexBasis));
    fb.items.add(juce::FlexItem(m_noiseAmountDial).withFlex(0, 1, flexBasis));
    fb.items.add(juce::FlexItem(m_delayTimeDial).withFlex(0, 1, flexBasis));
    fb.items.add(juce::FlexItem(m_delayDryWetDial).withFlex(0, 1, flexBasis));

    fb.performLayout (getLocalBounds());
}

}