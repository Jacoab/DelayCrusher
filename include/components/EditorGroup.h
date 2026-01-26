#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "components/ColorPalette.h"

namespace dlcr
{

/**
 * @class EditorGroup
 * @brief Base class grouping editor components with a title 
 * and outline box.
 * 
 */
class EditorGroup : public juce::Component
{
public:
    explicit EditorGroup(const std::string title);

    virtual ~EditorGroup() = default;

    /**
     * @brief Paints an outline box with the title around 
     * the dials.
     * 
     * @param g The graphics context to paint on.
     */
    void paint(juce::Graphics& g) override;

protected:
    const std::string m_title; /**< Title of the editor group */
};

}