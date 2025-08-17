#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CloudCrusherAudioProcessor::CloudCrusherAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                       )
#endif
{
}

CloudCrusherAudioProcessor::~CloudCrusherAudioProcessor() {}

const juce::String CloudCrusherAudioProcessor::getName() const { return JucePlugin_Name; }

bool CloudCrusherAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CloudCrusherAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CloudCrusherAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CloudCrusherAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int CloudCrusherAudioProcessor::getNumPrograms() { return 1; }
int CloudCrusherAudioProcessor::getCurrentProgram() { return 0; }
void CloudCrusherAudioProcessor::setCurrentProgram (int) {}
const juce::String CloudCrusherAudioProcessor::getProgramName (int) { return {}; }
void CloudCrusherAudioProcessor::changeProgramName (int, const juce::String&) {}

void CloudCrusherAudioProcessor::prepareToPlay (double, int) {}
void CloudCrusherAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CloudCrusherAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // Only allow stereo in/out
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif
    return true;
#endif
}
#endif

void CloudCrusherAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        buffer.clear (channel, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);
    m_processorChain.process(context);
}

bool CloudCrusherAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* CloudCrusherAudioProcessor::createEditor()
{
    return new CloudCrusherAudioProcessorEditor (*this);
}

void CloudCrusherAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store parameters in the memory block.
}

void CloudCrusherAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore parameters from this memory block.
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CloudCrusherAudioProcessor();
}
