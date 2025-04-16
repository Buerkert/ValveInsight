#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void switchingDetectionFixed_Reset(void);
void switchingDetectionFixed_StoreADC(int16_t adcValue);
double switchingDetectionFixed_Calculate(double timestep);

#ifdef __cplusplus
}
#endif
