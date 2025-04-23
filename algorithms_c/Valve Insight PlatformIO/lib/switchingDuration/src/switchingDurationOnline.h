#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** @brief Reset internal state. */
void switchingDurationOnline_Reset(void);

/**
 * Calculate the time until tail slope small.
 * @param adcValue - current measurement.
 * @param timestep - Time interval between samples.
 * @return Duration from start to band.
 */
double switchingDurationOnline_Calculate(int16_t adcValue, double timestep);

#ifdef __cplusplus
}
#endif
