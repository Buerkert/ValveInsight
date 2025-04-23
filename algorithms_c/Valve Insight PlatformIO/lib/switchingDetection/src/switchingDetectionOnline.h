#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** @brief Reset internal state. */
void switchingDetectionOnline_Reset(void);

double switchingDetectionOnline_Calculate(int16_t adcValue, double timestep);

#ifdef __cplusplus
}
#endif
