#include <cmath>

#include <gtest/gtest.h>

#include "BoxMullerNoise.h"

TEST(TestBoxMullerNoise, KolmogorovSmirnovTest)
{
    constexpr std::size_t numSamples = 10000;

    auto boxMullerNoise = glos::clcr::BoxMullerNoise();
    auto samples = boxMullerNoise.nextNSamples<numSamples>();
    std::sort(samples.begin(), samples.end());

    auto dMax = 0.0f;
    for (std::size_t i = 0; i < numSamples; ++i)
    {
        auto cdf = 0.5f * std::erfc(-samples[i] * static_cast<float>(M_SQRT1_2)); // CDF for standard normal distribution
        auto expectedCDF = static_cast<float>(i + 1) / numSamples; // Empirical CDF for sorted samples
        auto d = std::max(std::abs(expectedCDF - cdf), std::abs(cdf - static_cast<float>(i) / numSamples)); // K-S D statistic
        
        // Keep track of the maximum D statistic
        if (d > dMax) dMax = d;
    }

    // Kolmogorov-Smirnov critical value for given alpha = 0.05
    float ksCritical = 1.36f / std::sqrt(static_cast<float>(numSamples));

    ASSERT_TRUE(dMax < ksCritical);
}