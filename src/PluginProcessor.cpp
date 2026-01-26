#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace dlcr
{

DelayCrusherAudioProcessor::DelayCrusherAudioProcessor() :
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

DelayCrusherAudioProcessor::~DelayCrusherAudioProcessor() {}

const juce::String DelayCrusherAudioProcessor::getName() const { return "Delay Crusher"; }

bool DelayCrusherAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayCrusherAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayCrusherAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayCrusherAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int DelayCrusherAudioProcessor::getNumPrograms() { return 1; }
int DelayCrusherAudioProcessor::getCurrentProgram() { return 0; }
void DelayCrusherAudioProcessor::setCurrentProgram (int) {}
const juce::String DelayCrusherAudioProcessor::getProgramName (int) { return {}; }
void DelayCrusherAudioProcessor::changeProgramName (int, const juce::String&) {}

void DelayCrusherAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) 
{
    juce::dsp::ProcessSpec spec (
        sampleRate, 
        static_cast<uint32_t>(samplesPerBlock),
        static_cast<uint32_t>(getTotalNumOutputChannels())
    );

    auto& bitCrusher = m_processorChain.get<BitCrusherIndex>();
    bitCrusher.registerParameters(m_apvts);

    auto& noise = m_processorChain.get<NoiseIndex>();
    noise.registerParameters(m_apvts);

    auto& delay = m_processorChain.get<DelayIndex>();
    delay.registerParameters(m_apvts);

    m_processorChain.prepare(spec);
}

void DelayCrusherAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayCrusherAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DelayCrusherAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    if (buffer.getNumChannels() == 0 || buffer.getNumSamples() == 0)
        return;

    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);
    m_processorChain.process(context);
}

bool DelayCrusherAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* DelayCrusherAudioProcessor::createEditor()
{
    return new DelayCrusherAudioProcessorEditor (*this);
}

void DelayCrusherAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store parameters in the memory block.
}

void DelayCrusherAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore parameters from this memory block.
}

juce::AudioProcessorValueTreeState& DelayCrusherAudioProcessor::getAPVTS()
{
    return m_apvts;
}

const BitCrusher& DelayCrusherAudioProcessor::getBitCrusher()
{
    return m_processorChain.get<BitCrusherIndex>();
}

const BoxMullerNoise& DelayCrusherAudioProcessor::getNoiseGenerator()
{
    return m_processorChain.get<NoiseIndex>();
}

const Delay& DelayCrusherAudioProcessor::getDelay()
{
    return m_processorChain.get<DelayIndex>();
}

juce::AudioProcessorValueTreeState::ParameterLayout DelayCrusherAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto bitDepthMin = 1.0f;
    auto bitDepthMax = 16.0f;
    auto bitDepthStep = 1.0f;
    auto bitDepthDefault = bitDepthMax;
    auto bitDepthParam = std::make_unique<juce::AudioParameterFloat> (
        BitCrusher::BIT_DEPTH_DIAL_ID,
        BitCrusher::BIT_DEPTH_DIAL_TEXT,
        juce::NormalisableRange<float>(bitDepthMin, bitDepthMax, bitDepthStep),
        bitDepthDefault
    );

    auto sampleRateReduxMin = 1.0f;
    auto sampleRateReduxMax = 16.0f;
    auto sampleRateReduxStep = 1.0f;
    auto sampleRateReduxDefault = sampleRateReduxMin;
    auto sampleRateReduxParam = std::make_unique<juce::AudioParameterFloat> (
        BitCrusher::SAMPLE_RATE_REDUX_DIAL_ID,
        BitCrusher::SAMPLE_RATE_REDUX_DIAL_TEXT,
        juce::NormalisableRange<float>(sampleRateReduxMin, sampleRateReduxMax, sampleRateReduxStep),
        sampleRateReduxDefault
    );

    auto noiseAmountMin = 0.0f;
    auto noiseAmountMax = 1.0f;
    auto noiseAmountStep = 0.01f;
    auto noiseAmountDefault = noiseAmountMin;
    auto noiseAmountParam = std::make_unique<juce::AudioParameterFloat> (
        BoxMullerNoise::NOISE_AMOUNT_DIAL_ID,
        BoxMullerNoise::NOISE_AMOUNT_DIAL_TEXT,
        juce::NormalisableRange<float>(noiseAmountMin, noiseAmountMax, noiseAmountStep),
        noiseAmountDefault
    );

    auto delayTimeMin = 0.0f;
    auto delayTimeMax = 2000.0f;
    auto delayTimeStep = 1.0f;
    auto delayTimeDefault = delayTimeMin;
    auto delayTimeParam = std::make_unique<juce::AudioParameterFloat> (
        Delay::DELAY_TIME_DIAL_ID,
        Delay::DELAY_TIME_DIAL_TEXT,
        juce::NormalisableRange<float>(delayTimeMin, delayTimeMax, delayTimeStep),
        delayTimeDefault
    );

    auto dryWetMin = 0.0f;
    auto dryWetMax = 1.0f;
    auto dryWetStep = 0.01f;
    auto dryWetDefault = 0.5;
    auto dryWetParam = std::make_unique<juce::AudioParameterFloat> (
        Delay::DRY_WET_DIAL_ID,
        Delay::DRY_WET_DIAL_TEXT,
        juce::NormalisableRange<float>(dryWetMin, dryWetMax, dryWetStep),
        dryWetDefault
    );

    layout.add(std::move(bitDepthParam));
    layout.add(std::move(sampleRateReduxParam));
    layout.add(std::move(noiseAmountParam));
    layout.add(std::move(delayTimeParam));
    layout.add(std::move(dryWetParam));

    return layout;
}

}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new dlcr::DelayCrusherAudioProcessor();
}
