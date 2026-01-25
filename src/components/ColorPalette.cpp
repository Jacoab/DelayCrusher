#include "components/ColorPalette.h"

namespace glos::clcr
{

juce::Colour getColorFromPalette(ColorPalette color)
{
    return juce::Colour(static_cast<uint32_t>(color));
}

}