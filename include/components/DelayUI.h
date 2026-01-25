#pragma once

#include <cstdint>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "components/Dial.h"
#include "components/ColorPalette.h"
#include "processors/Delay.h"

namespace glos::clcr
{

/**
 * @class DelayUI
 * @brief Delay UI component containing dials for delay time and delay
 * dry/wet mix.
 * 
 */
class DelayUI : public juce::Component
{
public:
    /** Create a new DelayUI component.
     * 
     * @param apvts Reference to the AudioProcessorValueTreeState.
     */
    DelayUI(juce::AudioProcessorValueTreeState& apvts);

    /**
     * @brief Called when the component is resized.  
     * This will layout the dials.
     * 
     */
    void resized() override;

    /**
     * @brief Paints an outline box with the title "Delay" around 
     * the dials.
     * 
     * @param g The graphics context to paint on.
     */
    void paint(juce::Graphics& g) override;

private:
    Dial m_delayTimeDial;       /**< Dial to control delay time in milliseconds */
    Dial m_delayDryWetDial;     /**< Dial to control delay dry/wet mix */
};

}