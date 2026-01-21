#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "components/Dial.h"
#include "components/BitCrusherUI.h"
#include "components/DelayUI.h"
#include "processors/BitCrusher.h"
#include "processors/Delay.h"
#include "PluginProcessor.h"

namespace glos::clcr
{

/**
 * @class CloudCrusherAudioProcessorEditor
 * @brief Graphical editor for the CloudCrusher audio processor.
 *
 * This class implements the plugin's user interface. It owns and lays out
 * `Dial` components for each parameter exposed by the processor (bit depth,
 * sample-rate reduction, noise amount, delay time, and delay dry/wet) and
 * forwards user interactions to the underlying `CloudCrusherAudioProcessor`.
 */
class CloudCrusherAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    /**
     * @brief Construct the editor for the given processor instance.
     *
     * @param processor Reference to the processor this editor controls.
     */
    explicit CloudCrusherAudioProcessorEditor (CloudCrusherAudioProcessor& processor);

    /**
     * @brief Destructor.
     */
    ~CloudCrusherAudioProcessorEditor() override;

    /**
     * @brief Paint the editor UI.
     *
     * @param g Graphics context for drawing.
     */
    void paint (juce::Graphics& g) override;

    /**
     * @brief Called when the editor is resized; lays out child components.
     */
    void resized() override;

private:
    CloudCrusherAudioProcessor& m_audioProcessor; /**< Reference to the owning processor */

    BitCrusherUI m_bitCrusherUI; /**< UI for bit crusher controls */
    DelayUI m_delayUI;         /**< UI for delay controls */

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CloudCrusherAudioProcessorEditor)
};

}