#pragma once

#include <concepts>
#include <string_view>

#include <juce_audio_processors/juce_audio_processors.h>

namespace dlcr
{

// Source - https://stackoverflow.com/a
// Posted by manlio, modified by community. See post 'Timeline' for change history
// Retrieved 2025-11-15, License - CC BY-SA 4.0
/**
 * @class Numeric
 * @brief Concept to constrain a type to numeric types only.
 * 
 * @tparam T 
 */
template<class T> concept Numeric = std::is_arithmetic_v<T>;

/**
 * @struct ProcessorParam
 * @brief Template struct to represent a processor parameter synchronized 
 * with JUCE AudioProcessorValueTreeState.
 * 
 * @tparam id The unique identifier for the parameter.
 * @tparam name The user-facing name of the parameter.
 * @tparam ParamType The numeric type of the parameter value.
 */
template <
    const char* ID,
    const char* NAME,
    Numeric ParamType
>
class ProcessorParam : public juce::AudioProcessorValueTreeState::Listener
{
public:
    /**
     * @brief Construct a new ProcessorParam object.
     * 
     * @param initialValue Initial value for the parameter.
     */
    explicit ProcessorParam(ParamType initialValue) : 
        m_value(initialValue) 
    {
    }

    virtual ~ProcessorParam() = default;

    /**
     * @brief Called when a parameter value is changed in the AudioProcessorValueTreeState.
     * This updates the internal atomic value to reflect the new parameter value.
     * 
     * @param parameterID The ID of the parameter that changed.
     * @param newValue The new value of the parameter.
     */
    void parameterChanged (const juce::String& parameterID, float newValue) override
    {
        if (parameterID.compare(ID) == 0)
        {
            m_value.store(static_cast<ParamType>(newValue));
        }
    }

    /**
     * @brief Set the parameter value.  This member function is thread-safe.
     * 
     * @param newValue The new value to set.
     */
    void store(ParamType newValue) noexcept
    {
        m_value.store(newValue);
    }

    /**
     * @brief Get the parameter value.  This member function is thread-safe.
     * 
     * @return ParamType The current value of the parameter.
     */
    ParamType load() const
    {
        return m_value.load();
    }

    static constexpr const char* id = ID;   /**< Parameter ID */
    static constexpr const char* name = NAME; /**< Parameter name */

private:
    std::atomic<ParamType> m_value; /**< Current value of the parameter */
};

}