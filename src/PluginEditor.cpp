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
    auto area = getLocalBounds().reduced(20);
    auto knobWidth = area.getWidth() / 5;

    m_sampleRateReduxDial.setBounds(area.removeFromLeft(knobWidth));
    m_bitDepthDial.setBounds(area.removeFromLeft(knobWidth));
    m_noiseAmountDial.setBounds(area.removeFromLeft(knobWidth));
    m_delayTimeDial.setBounds(area.removeFromLeft(knobWidth));
    m_delayDryWetDial.setBounds(area);
}

}