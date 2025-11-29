#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace glos::clcr
{

/**
 * @class Dial
 * @brief Dial UI component with label and attachment to AudioProcessorValueTreeState parameter.
 * 
 */
class Dial : public juce::Component
{
public:
    /** Create a new Dial component.
     * 
     * @param name The name of the dial (used for the label).
     * @param apvts Reference to the AudioProcessorValueTreeState.
     * @param paramID The parameter ID to attach the dial to.
     */
    Dial(const juce::String& name,
         juce::AudioProcessorValueTreeState& apvts,
         const juce::String& paramID);

    /**
     * @brief Called when the component is resized.  
     * This will layout the slider and label.
     * 
     */
    void resized() override;

private:
    juce::Slider m_slider; /**< Rotary slider */
    juce::Label m_label; /**< Label of the slider*/
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_attachment; /**< Attachment to APVTS parameter */
};

}