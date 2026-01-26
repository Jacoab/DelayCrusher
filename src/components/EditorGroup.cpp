#include "EditorGroup.h"

namespace glos::clcr
{

EditorGroup::EditorGroup(const std::string title) :
    m_title(title)
{
}

void EditorGroup::paint(juce::Graphics& g)
{
    constexpr auto cornerRadius = 6.0f;
    constexpr auto stroke = 1.5f;

    auto primaryVariantColor = getColorFromPalette(ColorPalette::PrimaryVariant);
    auto backgroundColor = getColorFromPalette(ColorPalette::Background);

    g.setFont (14.0f);
    const auto titleHeight = juce::roundToInt (g.getCurrentFont().getHeight());

    auto bounds = getLocalBounds().toFloat();

    auto box = bounds;
    box.setY (box.getY() + titleHeight * 0.5f);
    box.setHeight (box.getHeight() - titleHeight * 0.5f);

    g.setColour(primaryVariantColor);
    g.drawRoundedRectangle(
        box.reduced (stroke * 0.5f),
        cornerRadius,
        stroke
    );

    const auto textWidth = g.getCurrentFont().getStringWidth(m_title);
    const auto textX = static_cast<int> (bounds.getCentreX() - textWidth * 0.5f);
    const auto textY = 0;

    g.setColour(backgroundColor);
    g.fillRect(
        textX - 6,
        textY,
        textWidth + 12,
        titleHeight
    );

    g.setColour(primaryVariantColor);
    g.drawText(
        m_title,
        textX,
        textY,
        textWidth,
        titleHeight,
        juce::Justification::centred,
        false
    );
}

}