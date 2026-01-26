#pragma once

#include <cstdint>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "components/EditorGroup.h"
#include "components/Dial.h"
#include "components/ColorPalette.h"
#include "processors/Delay.h"

namespace dlcr
{

/**
 * @class DelayUI
 * @brief Delay UI component containing dials for delay time and delay
 * dry/wet mix.
 * 
 */
class DelayUI : public EditorGroup
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

private:
    Dial m_delayTimeDial;       /**< Dial to control delay time in milliseconds */
    Dial m_delayDryWetDial;     /**< Dial to control delay dry/wet mix */
};

}