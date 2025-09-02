#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace glos::clcr
{

class Dial : public juce::Component
{
public:
    Dial(const juce::String& name,
         juce::AudioProcessorValueTreeState& apvts,
         const juce::String& paramID);

    void resized() override;

private:
    juce::Slider m_slider;
    juce::Label m_label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_attachment;
};

}