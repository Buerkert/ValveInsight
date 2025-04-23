#include "switchingDetectionFixed.h"


#include <math.h>
#include <string.h>
#include <stdbool.h>


#define INRUSH_CURRENT_VALUES        2000
#define TAIL_WINDOW_SIZE             200
#define TAIL_WINDOW_START_INITIAL    200
// #define SLOPE_THRESHOLD              0.002
// If current is scaled by 500 to match uint16_t range, then slope threshold is 1.0
#define SLOPE_THRESHOLD              1.0

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

double round2(double x) {
    return round(x * 100.0) / 100.0;
}


void switchingDetectionTailSlope_Reset(void) {
	n = 0;
	memset(current, 0, sizeof(current));
}

void switchingDetectionTailSlope_StoreADC(int16_t adcValue) {
	// Store current adc value
	if(n < INRUSH_CURRENT_VALUES) {
		current[n] = adcValue;
	}
	n = safe_increment(n);
}

// Calculate the slope of a linear regression line for equidistant data points
// see e.g. https://www.embeddedrelated.com/showarticle/1163.php
double linearRegressionSlopeEquidistant(double timestep, const int16_t *y, size_t windowStart, size_t n)
{
    if (n < 2 || timestep == 0.0 || windowStart + n > INRUSH_CURRENT_VALUES) {
        return 0.0;
    }

    double sum = 0.0;
    double mid = (double)(n - 1) / 2.0;

    for (uint16_t i = 0; i < n; ++i) {
        sum += (i - mid) * (double)(y[windowStart + i]);
    }

    double factor = 12.0 / (n * (n * n - 1));
    return factor * sum / timestep;
}

double switchingDetectionTailSlope_Calculate(double timestep) {
	// Calculate tail mean
	size_t tailWindowStart = TAIL_WINDOW_START_INITIAL;

	bool tailConst = false;
	while (!tailConst) {
		double slope = linearRegressionSlopeEquidistant(timestep, current, tailWindowStart, TAIL_WINDOW_SIZE);

		if (fabs(slope) < SLOPE_THRESHOLD) {
			tailConst = true;
		} else if (tailWindowStart + TAIL_WINDOW_SIZE >= INRUSH_CURRENT_VALUES) {
			break;  // reached end of buffer
		} else if (tailWindowStart + TAIL_WINDOW_SIZE >= n) {
			break; // reached end of data
		} else {
			tailWindowStart += (size_t)(1 / timestep);  // forward by 1 ms
		}
	}

	double tailMean = 0;
	for (uint16_t i = tailWindowStart; i < tailWindowStart + TAIL_WINDOW_SIZE; ++i) {
		tailMean += (double)current[i];
	}

	tailMean /= TAIL_WINDOW_SIZE;

	// Calculate integral
	double integral = 0;
	for (uint16_t i = 0; i < tailWindowStart + TAIL_WINDOW_SIZE; ++i) {
		integral += (tailMean - (double)current[i]) / tailMean;
	}
	integral *= timestep;
	integral = fabs(integral); 
	integral = round2(integral);

	return integral;
}
