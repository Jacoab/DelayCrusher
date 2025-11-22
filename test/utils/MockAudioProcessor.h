#pragma once

#include <gmock/gmock.h>
#include <juce_audio_processors/juce_audio_processors.h>

class MockAudioProcessor : public juce::AudioProcessor
{
public:
    MockAudioProcessor() 
        : AudioProcessor(BusesProperties()
            .withInput("In", juce::AudioChannelSet::stereo(), true)
            .withOutput("Out", juce::AudioChannelSet::stereo(), true))
    {
    }

    ~MockAudioProcessor() override = default;

    MOCK_METHOD(const juce::String, getName, (), (const, override));
    MOCK_METHOD(void, prepareToPlay, (double, int), (override));
    MOCK_METHOD(void, releaseResources, (), (override));
    MOCK_METHOD(bool, isBusesLayoutSupported, (const BusesLayout&), (const, override));
    MOCK_METHOD(void, processBlock, (juce::AudioBuffer<float>&, juce::MidiBuffer&), (override));
    MOCK_METHOD(juce::AudioProcessorEditor*, createEditor, (), (override));
    MOCK_METHOD(bool, hasEditor, (), (const, override));
    MOCK_METHOD(bool, acceptsMidi, (), (const, override));
    MOCK_METHOD(bool, producesMidi, (), (const, override));
    MOCK_METHOD(bool, isMidiEffect, (), (const, override));
    MOCK_METHOD(double, getTailLengthSeconds, (), (const, override));
    MOCK_METHOD(int, getNumPrograms, (), (override));
    MOCK_METHOD(int, getCurrentProgram, (), (override));
    MOCK_METHOD(void, setCurrentProgram, (int), (override));
    MOCK_METHOD(const juce::String, getProgramName, (int), (override));
    MOCK_METHOD(void, changeProgramName, (int, const juce::String&), (override));
    MOCK_METHOD(void, getStateInformation, (juce::MemoryBlock&), (override));
    MOCK_METHOD(void, setStateInformation, (const void*, int), (override));
};
