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


void switchingDurationInsideBand_Reset(void) {
	n = 0;
	memset(current, 0, sizeof(current));
}

void switchingDurationInsideBand_StoreADC(int16_t adcValue) {
	// Store current adc value
	if(n < INRUSH_CURRENT_VALUES) {
		current[n] = adcValue;
	}
	n = safe_increment(n);
}

double switchingDurationInsideBand_Calculate(double timestep, double bandwidth) {
	// Calculate tail mean
	double tailMean = 0;

	for (uint16_t i = INRUSH_CURRENT_VALUES - TAIL_WINDOW_SIZE; i < INRUSH_CURRENT_VALUES; ++i) {
		tailMean += (double)current[i];
	}

	tailMean /= TAIL_WINDOW_SIZE;

    double currentLowerBand = tailMean - tailMean*bandwidth;
    double currentUpperBand = tailMean + tailMean*bandwidth;

    size_t firstIndexInsideBand = INRUSH_CURRENT_VALUES;
    size_t lastGoodIndex = INRUSH_CURRENT_VALUES;

    // Start from last index and go backwards to find the first value outside the band
    size_t len = sizeof(current) / sizeof(current[0]);
    for (size_t idx = len-1; idx > 0; --idx) {
        double I = (double)current[idx];

        if (I < currentLowerBand || I > currentUpperBand) {
            firstIndexInsideBand = lastGoodIndex;
            break;
        }
        lastGoodIndex = idx;
    }

    double duration = firstIndexInsideBand*timestep;

	return duration;
}
