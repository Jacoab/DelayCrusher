#include "components/ColorPalette.h"

namespace dlcr
{

juce::Colour getColorFromPalette(ColorPalette color)
{
    return juce::Colour(static_cast<uint32_t>(color));
}

}