#include <gtest/gtest.h>

#include "BitCrusher.h"
#include "utils/TestNoise.h"

TEST(TestBitCrusher, ProcessSample)
{
    auto modVal = 0.0f;
    auto linearModifier = [&modVal]() 
    { 
        modVal += 0.1f;
        return modVal;
    };
    auto testNoise = TestNoise(1.0f, linearModifier);

    auto sampleRateRedux = 2;
    auto bitDepth = 8;
    glos::clcr::BitCrusher<TestNoise> bitCrusher(sampleRateRedux, bitDepth, testNoise);
    bitCrusher.setNoiseAmount(0.5f);

    auto expectedProcessedSample = 0.24921875f;
    auto processedSample = bitCrusher.process(0.2f);
    
    ASSERT_NEAR(processedSample, expectedProcessedSample, 0.0001f);
}