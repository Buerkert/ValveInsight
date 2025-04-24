#include "switchingDurationInsideBand.h"

#include <math.h>
#include <string.h>


#define INRUSH_CURRENT_VALUES     500
#define TAIL_WINDOW_SIZE          100

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


void switchingDurationIntegralThreshold_Reset(void) {
	n = 0;
	memset(current, 0, sizeof(current));
}

void switchingDurationIntegralThreshold_StoreADC(int16_t adcValue) {
	// Store current adc value
	if(n < INRUSH_CURRENT_VALUES) {
		current[n] = adcValue;
	}
	n = safe_increment(n);
}

double switchingDurationIntegralThreshold_Calculate(double timestep, double threshold) {
	// Calculate tail mean
	double tailMean = 0;

	for (uint16_t i = INRUSH_CURRENT_VALUES - TAIL_WINDOW_SIZE; i < INRUSH_CURRENT_VALUES; ++i) {
		tailMean += (double)current[i];
	}

	tailMean /= TAIL_WINDOW_SIZE;

    double integral = 0;
    size_t durationIdx = INRUSH_CURRENT_VALUES-1;
	for (uint16_t i = 0; i < INRUSH_CURRENT_VALUES; ++i) {
        double currentInv = tailMean - (double)current[i];
        double currentInvNorm = currentInv / tailMean;
        integral += currentInvNorm * timestep;

        if (integral >= threshold) {
            durationIdx = i;
            break;
        }
	}

    double duration = durationIdx * timestep;

	return duration;
}
