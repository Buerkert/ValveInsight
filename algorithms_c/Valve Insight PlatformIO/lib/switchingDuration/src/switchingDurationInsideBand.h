#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Reset the internal state (e.g., clears stored ADC values).
 */
void switchingDurationInsideBand_Reset(void);

/**
 * Store a new ADC value for processing.
 * @param adcValue - 16-bit signed ADC reading.
 */
void switchingDurationInsideBand_StoreADC(int16_t adcValue);

/**
 * Calculate the time until the current signal stays within the specified band.
 * @param timestep - Time interval between samples.
 * @param bandwidth - Band around the tail mean.
 * @return Duration from start to band.
 */
double switchingDurationInsideBand_Calculate(double timestep, double bandwidth);

#ifdef __cplusplus
}
#endif
