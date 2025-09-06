#include "PluginEditor.h"

namespace glos::clcr
{

CloudCrusherAudioProcessorEditor::CloudCrusherAudioProcessorEditor (CloudCrusherAudioProcessor& p) : 
    AudioProcessorEditor (&p), 
    m_audioProcessor (p),
    m_sampleRateReduxDial(SAMPLE_RATE_REDUX_DIAL_TEXT, m_audioProcessor.getAPVTS(), SAMPLE_RATE_REDUX_DIAL_ID),
    m_bitDepthDial(BIT_DEPTH_DIAL_TEXT, m_audioProcessor.getAPVTS(), BIT_DEPTH_DIAL_ID),
    m_noiseAmountDial(NOISE_AMOUNT_DIAL_TEXT, m_audioProcessor.getAPVTS(), NOISE_AMOUNT_DIAL_ID)
{
    setSize (400, 300);

    addAndMakeVisible(m_sampleRateReduxDial);
    addAndMakeVisible(m_bitDepthDial);
    addAndMakeVisible(m_noiseAmountDial);
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
    auto knobWidth = area.getWidth() / 3;

    m_sampleRateReduxDial.setBounds(area.removeFromLeft(knobWidth));
    m_bitDepthDial.setBounds(area.removeFromLeft(knobWidth));
    m_noiseAmountDial.setBounds(area);
}

}