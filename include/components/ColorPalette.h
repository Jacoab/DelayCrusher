#pragma once

#include <cstdint>

#include <juce_gui_basics/juce_gui_basics.h>

namespace glos::clcr
{

/**
 * @enum ColorPalette
 * @brief Enum class defining the color palette used in the UI.
 * 
 */
enum class ColorPalette : uint32_t
{
    Primary = 0xfffeecc3,
    PrimaryVariant = 0xfffeecc3,
    Secondary = 0xff333a44,
    SecondaryVariant = 0xff333a44,
    Background = 0xff333a44,
};

/**
 * @brief Get a JUCE Colour object from the ColorPalette enum.
 * 
 * @param color The ColorPalette enum value.
 * @return juce::Colour The corresponding JUCE Colour object.
 */
juce::Colour getColorFromPalette(ColorPalette color);

}
