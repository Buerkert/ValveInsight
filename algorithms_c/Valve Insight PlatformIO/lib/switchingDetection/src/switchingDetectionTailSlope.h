#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** @brief Reset internal state and clear ADC buffer. */
void switchingDetectionTailSlope_Reset(void);

void switchingDetectionTailSlope_StoreADC(int16_t adcValue);

double switchingDetectionTailSlope_Calculate(double timestep);

double linearRegressionSlopeEquidistant(double timestep, const int16_t *y, size_t windowStart, size_t n);

#ifdef __cplusplus
}
#endif
