#include "ConstNoise.h"

ConstNoise::ConstNoise() : 
    m_noiseSample(0.01f)
{
}

ConstNoise::ConstNoise(float noiseSample) : 
    m_noiseSample(noiseSample)
{
}


float ConstNoise::nextSample() noexcept
{
    return m_noiseSample;
} 

juce::AudioBuffer<float>& ConstNoise::nextNSamples(int n) noexcept
{
    m_noiseBuffer.setSize(1, n);
    auto* writePointer = m_noiseBuffer.getWritePointer(0);
    
    for (std::size_t i = 0; i < n; ++i)
    {
        writePointer[i] = nextSample();
    }
    
    return m_noiseBuffer;
}