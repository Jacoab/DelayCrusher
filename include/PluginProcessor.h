#pragma once

#include <cstdint>

#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "processors/BoxMullerNoise.h"
#include "processors/BitCrusher.h"
#include "processors/Delay.h"

namespace glos::clcr
{

/**
 * @class CloudCrusherAudioProcessor
 * @brief Main audio processor for the CloudCrusher plugin.
 *
 * This class implements juce::AudioProcessor and manages the plugin's
 * DSP processor chain (BitCrusher, BoxMullerNoise, Delay) as well as the
 * AudioProcessorValueTreeState that holds parameters used by both the UI
 * and the audio processing code.
 */
class CloudCrusherAudioProcessor  : public juce::AudioProcessor
{
public:
    /** 
     * @brief Constructor. Initializes the processor chain and parameter state.
     */
    CloudCrusherAudioProcessor();

    /** 
     * @brief Destructor. 
     */
    ~CloudCrusherAudioProcessor() override;

    /**
     * @brief Prepare resources before playback begins.
     * @param sampleRate Current sample rate in Hz.
     * @param samplesPerBlock Expected maximum block size.
     */
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;

    /** 
     * @brief Release any resources allocated in prepareToPlay. 
     */
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    /**
     * @brief Indicates whether the given bus layout is supported.
     * @param layouts The proposed bus layout.
     * @return true if the layout is supported, false otherwise.
     */
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    /**
     * @brief Main audio processing callback.
     * @param buffer Audio buffer containing input/output samples.
     * @param midiMessages MIDI events for the current block.
     */
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    /** 
     * @brief Create and return the plugin editor. 
     */
    juce::AudioProcessorEditor* createEditor() override;

    /** 
     * @brief Whether this processor has an editor. 
     */
    bool hasEditor() const override;

    /** 
     * @brief Gets the name of the plugin.
     * @return The display name of the plugin. 
     */
    const juce::String getName() const override;

    /** 
     * @brief Whether this processor accepts incoming MIDI. 
     * 
     * @return true if MIDI input is supported, false otherwise. 
     */
    bool acceptsMidi() const override;

    /** 
     * @brief Whether this processor produces MIDI output. 
     * @return true if MIDI output is produced, false otherwise. 
     */
    bool producesMidi() const override;

    /** 
     * @brief Whether this processor is a MIDI effect. 
     * 
     * @return true if this is a MIDI-only effect, false otherwise. 
     */
    bool isMidiEffect() const override;

    /** 
     * @brief How long the processor continues to produce sound after input stops. 
     * 
     * @return Tail length in seconds. 
     */
    double getTailLengthSeconds() const override;

    /** 
     * @brief Stubbed override: number of programs. This plugin uses a single program. 
     */
    int getNumPrograms() override;

    /** 
     * @brief Stubbed override: current program index. 
     */
    int getCurrentProgram() override;

    /** 
     * @brief Stubbed override: set current program (no-op). 
     * 
     * @param index Program index to set.
     */
    void setCurrentProgram (int index) override;

    /** 
     * @brief Stubbed override: get program name (no programs). 
     * 
     * @param index Program index. 
     * @return Program name or empty string. 
     */
    const juce::String getProgramName (int index) override;

    /** 
     * @brief Stubbed override: change program name (no-op). 
     * 
     * @param index Program index. 
     * @param newName New name. 
     */
    void changeProgramName (int index, const juce::String& newName) override;

    /**
     * @brief Save plugin state into a block.
     * @param destData Destination memory block.
     */
    void getStateInformation (juce::MemoryBlock& destData) override;
    /**
     * @brief Restore plugin state from a block.
     * @param data Pointer to state memory.
     * @param sizeInBytes Size of the state data in bytes.
     */
    void setStateInformation (const void* data, int sizeInBytes) override;

    /**
     * @brief Accessor for the APVTS that stores parameters for UI and DSP.
     * @return Reference to the AudioProcessorValueTreeState.
     */
    juce::AudioProcessorValueTreeState& getAPVTS();

    /**
     * @brief Const accessor to the BitCrusher processor in the chain.
     * @return Const reference to the BitCrusher.
     */
    const glos::clcr::BitCrusher& getBitCrusher();

    /**
     * @brief Const accessor to the noise generator in the chain.
     * @return Const reference to the BoxMullerNoise generator.
     */
    const glos::clcr::BoxMullerNoise& getNoiseGenerator();

    /**
     * @brief Const accessor to the Delay processor in the chain.
     * @return Const reference to the Delay processor.
     */
    const glos::clcr::Delay& getDelay();
    
    /**
     * @brief Create the parameter layout used by the APVTS.
     * @return A ParameterLayout describing all plugin parameters.
     */
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    enum {
        BitCrusherIndex,
        NoiseIndex,
        DelayIndex
    }; /**< Indices for processors in the processor chain. */
    
    juce::dsp::ProcessorChain<BitCrusher, BoxMullerNoise, Delay> m_processorChain; /**< Processor chain that performs the DSP in-series. */
    juce::AudioProcessorValueTreeState m_apvts; /**< AudioProcessorValueTreeState that stores plugin parameters. */

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CloudCrusherAudioProcessor)
};

}