#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/** @brief Reset internal state and clear ADC buffer. */
void switchingDetectionFixed_Reset(void);

bool switchingDetectionFixed_StoreADC(int16_t adcValue);
float switchingDetectionFixed_Calculate(float timestep);

#ifdef __cplusplus
}
#endif
