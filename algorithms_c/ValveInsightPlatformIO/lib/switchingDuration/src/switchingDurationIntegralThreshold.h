#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** @brief Reset internal state and clear ADC buffer. */
void switchingDurationIntegralThreshold_Reset(void);

/**
 * Store a new ADC value for processing.
 * @param adcValue - 16-bit signed ADC reading.
 */
void switchingDurationIntegralThreshold_StoreADC(int16_t adcValue);

/**
 * Calculate the time until the current signal stays within the specified band.
 * @param timestep - Time interval between samples.
 * @param threshold - Integral value threshold.
 * @return Duration from start to band.
 */
double switchingDurationIntegralThreshold_Calculate(double timestep, double threshold);

#ifdef __cplusplus
}
#endif
