#include "switchingDetectionFixed.h"


#include <math.h>
#include <string.h>

#define INRUSH_CURRENT_VALUES     500
#define TAIL_WINDOW_SIZE          200

static size_t n;
static int16_t current[INRUSH_CURRENT_VALUES];
// avoid increment overflow -> do not allow buffer size greater than SIZE_MAX
_Static_assert(INRUSH_CURRENT_VALUES < SIZE_MAX, "Buffer is too big!");

void switchingDetectionFixed_Reset(void) {
	n = 0;
	memset(current, 0, sizeof(current));
}

bool switchingDetectionFixed_fast_StoreADC(int16_t adcValue) {
	// Store current adc value
	if(n < INRUSH_CURRENT_VALUES) {
		current[n] = adcValue;
	  ++n;
    return true;
	}
	// buffer is full
  return false;
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

	integral = INRUSH_CURRENT_VALUES - (float)sum / tailMean ;
	integral *= timestep;
	integral = fabs(integral); 

	return integral;
}

// Explanation of the integral calculation
// integral += (tailMean - (float)current[i]) / tailMean;
// integral += tailMean / tailMean - (float)current[i] / tailMean;
// integral += 1 - (float)current[i] / tailMean;
// integral = INRUSH_CURRENT_VALUES - sum / tailMean;
