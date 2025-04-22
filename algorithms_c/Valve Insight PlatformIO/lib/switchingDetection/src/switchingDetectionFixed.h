#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** @brief Reset internal state and clear ADC buffer. */
void switchingDetectionFixed_Reset(void);

void switchingDetectionFixed_StoreADC(int16_t adcValue);
<<<<<<< HEAD

double switchingDetectionFixed_Calculate(double timestep);
=======
float switchingDetectionFixed_Calculate(float timestep);
>>>>>>> 1fc81d5 (used single precision (float) datatype)

#ifdef __cplusplus
}
#endif
