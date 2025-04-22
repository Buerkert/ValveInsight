#include "switchingDetectionFixed.h"


#include <math.h>
#include <string.h>

#define INRUSH_CURRENT_VALUES     500
#define TAIL_WINDOW_SIZE          200

static size_t n;
static int16_t current[INRUSH_CURRENT_VALUES];


// Increment without overflow 
size_t safe_increment(size_t x) {
    if (x < SIZE_MAX) {
        return x + 1;
    } else {
        return x;
    }
}

float round2(float x) {
    return round(x * 100.0) / 100.0;
}


void switchingDetectionFixed_Reset(void) {
	n = 0;
	memset(current, 0, sizeof(current));
}

void switchingDetectionFixed_StoreADC(int16_t adcValue) {
	// Store current adc value
	if(n < INRUSH_CURRENT_VALUES) {
		current[n] = adcValue;
	}
	n = safe_increment(n);
}

float switchingDetectionFixed_Calculate(float timestep) {
	// Calculate tail mean
	float tailMean = 0;

	for (uint16_t i = INRUSH_CURRENT_VALUES - TAIL_WINDOW_SIZE; i < INRUSH_CURRENT_VALUES; ++i) {
		tailMean += (float)current[i];
	}

	tailMean /= TAIL_WINDOW_SIZE;

	// Calculate integral
	float integral = 0;
	for (uint16_t i = 0; i < INRUSH_CURRENT_VALUES; ++i) {
		integral += (tailMean - (float)current[i]) / tailMean;
	}
	integral *= timestep;
	integral = fabs(integral); 
	integral = round2(integral);

	return integral;
}
