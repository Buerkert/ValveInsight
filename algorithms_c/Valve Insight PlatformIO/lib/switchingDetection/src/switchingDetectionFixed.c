#include "switchingDetectionFixed.h"


#include <math.h>
#include <string.h>

#define INRUSH_CURRENT_VALUES     500
#define TAIL_WINDOW_SIZE          200

static size_t n;
static int16_t current[INRUSH_CURRENT_VALUES];

float round2(float x) {
  return round(x * 100.0) / 100.0;
}

void switchingDetectionFixed_Reset(void) {
	n = 0;
	memset(current, 0, sizeof(current));
}

void switchingDetectionFixed_StoreADC(int16_t adcValue) {
  // Store current adc value. If the buffer is full, stop writing values.
  if(n < INRUSH_CURRENT_VALUES) {
    current[n] = adcValue;
    ++n;
  }
}

float switchingDetectionFixed_Calculate(float timestep) {
	// Calculate tail mean
	int32_t sum = 0;

	for (uint16_t i = INRUSH_CURRENT_VALUES - TAIL_WINDOW_SIZE; i < INRUSH_CURRENT_VALUES; ++i) {
		sum += current[i];
	}

	float tailMean = (float)sum / TAIL_WINDOW_SIZE;

  // Calculate integral
  float integral = 0;
  // tail sum is already in 'sum' so add only the missing values before the tail
  for (uint16_t i = 0; i < INRUSH_CURRENT_VALUES - TAIL_WINDOW_SIZE; ++i) {
    sum += current[i];
  }

// Explanation of the simplification of the integral calculation
// 1. integral += (tailMean - (float)current[i]) / tailMean;
// 2. integral += tailMean / tailMean - (float)current[i] / tailMean;
// 3. integral += 1 - (float)current[i] / tailMean;
// 4. integral = INRUSH_CURRENT_VALUES - sum / tailMean;

	integral = INRUSH_CURRENT_VALUES - (float)sum / tailMean ;
	integral *= timestep;
	integral = fabs(integral); 
  integral = round2(integral);

	return integral;
}
