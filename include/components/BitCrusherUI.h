#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "components/Dial.h"
#include "processors/BitCrusher.h"

namespace glos::clcr
{

/**
 * @class BitCrusherUI
 * @brief BitCrusher UI component containing dials for sample-rate reduction,
 * bit depth, and noise amount.
 * 
 */
class BitCrusherUI : public juce::Component
{
public:
    /** Create a new BitCrusherUI component.
     * 
     * @param apvts Reference to the AudioProcessorValueTreeState.
     */
    explicit BitCrusherUI(juce::AudioProcessorValueTreeState& apvts);

    /**
     * @brief Called when the component is resized.  
     * This will layout the dials.
     * 
     */
    void resized() override;

private:
    Dial m_sampleRateReduxDial; /**< Dial to control sample-rate reduction */
    Dial m_bitDepthDial;        /**< Dial to control bit depth */
    Dial m_noiseAmountDial;     /**< Dial to control noise amount */
};

}