#pragma once

#include <concepts>

#include <juce_core/juce_core.h>

namespace glos::clcr
{

/**
 * @brief Concept for noise generator types.
 * 
 * A type satisfies NoiseGenType if it has a method `nextSample()` that returns a float.
 */
template <typename T>
concept NoiseGenType = requires(T noiseGen)
{
    { noiseGen.nextSample() } -> std::convertible_to<float>;
    //{ noiseGen.nextNSamples(int n)} -> std::convertible_to<juce::AudioBuffer<float>&>;
};

} // gloss::clcr