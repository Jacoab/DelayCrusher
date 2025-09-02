#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CloudCrusherAudioProcessor::CloudCrusherAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                       ),
#endif
    m_apvts(*this, nullptr, "UI Parameters", createParameterLayout())
{
}

CloudCrusherAudioProcessor::~CloudCrusherAudioProcessor() {}

const juce::String CloudCrusherAudioProcessor::getName() const { return "Cloud Crusher"; }

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

void CloudCrusherAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) 
{
    juce::dsp::ProcessSpec spec (
        sampleRate, 
        static_cast<uint32_t>(samplesPerBlock),
        static_cast<uint32_t>(getTotalNumOutputChannels())
    );

    auto& bitCrusher = m_processorChain.get<BitCrusherIndex>();

    bitCrusher.init(getTotalNumOutputChannels(), static_cast<int>(sampleRate));

    bitCrusher.setSampleRateRedux(m_apvts.getRawParameterValue(glos::clcr::SAMPLE_RATE_REDUX_DIAL_ID));
    bitCrusher.setBitDepth(m_apvts.getRawParameterValue(glos::clcr::BIT_DEPTH_DIAL_ID));
    bitCrusher.setNoiseAmount(m_apvts.getRawParameterValue(glos::clcr::NOISE_AMOUNT_DIAL_ID));

    m_processorChain.prepare(spec);
}

void CloudCrusherAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CloudCrusherAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // Allow mono and stereo output
    auto mainOut = layouts.getMainOutputChannelSet();
    if (mainOut != juce::AudioChannelSet::mono() && mainOut != juce::AudioChannelSet::stereo())
        return false;
   #if ! JucePlugin_IsSynth
    if (mainOut != layouts.getMainInputChannelSet())
        return false;
   #endif
    return true;
#endif
}
#endif

void CloudCrusherAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    if (buffer.getNumChannels() == 0 || buffer.getNumSamples() == 0)
        return;

    bool hasInput = false;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            if (buffer.getSample(ch, i) != 0.0f)
                hasInput = true;

    // Optional: log or breakpoint here
    // DBG("Buffer has input: " << hasInput);

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

juce::AudioProcessorValueTreeState& CloudCrusherAudioProcessor::getAPVTS()
{
    return m_apvts;
}

const glos::clcr::GaussianBitCrusher& CloudCrusherAudioProcessor::getBitCrusher()
{
    return m_processorChain.get<BitCrusherIndex>();
}

juce::AudioProcessorValueTreeState::ParameterLayout CloudCrusherAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto bitDepthParam = std::make_unique<juce::AudioParameterFloat> (
        glos::clcr::BIT_DEPTH_DIAL_ID,
        glos::clcr::BIT_DEPTH_DIAL_TEXT,
        juce::NormalisableRange<float>(1.0f, 32.0f, 1.0f),
        32.0f
    );

    auto sampleRateReduxParam = std::make_unique<juce::AudioParameterFloat> (
        glos::clcr::SAMPLE_RATE_REDUX_DIAL_ID,
        glos::clcr::SAMPLE_RATE_REDUX_DIAL_TEXT,
        juce::NormalisableRange<float>(1.0f, 16.0f, 1.0f),
        1.0f
    );

    auto noiseAmountParam = std::make_unique<juce::AudioParameterFloat> (
        glos::clcr::NOISE_AMOUNT_DIAL_ID,
        glos::clcr::NOISE_AMOUNT_DIAL_TEXT,
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.0f
    );

    layout.add(std::move(bitDepthParam));
    layout.add(std::move(sampleRateReduxParam));
    layout.add(std::move(noiseAmountParam));

    return layout;
}
