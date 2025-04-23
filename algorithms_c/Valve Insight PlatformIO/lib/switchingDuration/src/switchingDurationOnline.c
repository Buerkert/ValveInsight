#include "switchingDurationInsideBand.h"

#include <math.h>
#include <string.h>

#define TAIL_WINDOW_SIZE             200
// #define SLOPE_THRESHOLD              0.2
// If current is scaled by 500 to match uint16_t range, scale threshold
#define SLOPE_THRESHOLD              100.0

static size_t n;
static int16_t tailWindow[TAIL_WINDOW_SIZE];

// Calculate the slope of a linear regression line for equidistant data points
// see e.g. https://www.embeddedrelated.com/showarticle/1163.php
double linearRegressionSlopeEquidistant(double timestep, const int16_t *y, size_t len, size_t n)
{
    if (len < 2 || timestep == 0.0) {
        return 0.0;
    }

    double sum = 0.0;
    double mid = (double)(len - 1) / 2.0;

	size_t windowIdx = (n) % TAIL_WINDOW_SIZE;  // this is the oldest element in the window
    for (uint16_t i = 0; i < len; ++i) {
        sum += (i - mid) * (double)(y[windowIdx]);
		windowIdx = (windowIdx + 1) % TAIL_WINDOW_SIZE;
    }

    double factor = 12.0 / (len * (len * len - 1));
    return factor * sum / timestep;
}

void switchingDurationOnline_Reset(void) {
	n = 0;
	memset(tailWindow, 0, sizeof(tailWindow));
}

double switchingDurationOnline_Calculate(int16_t adcValue, double timestep) {
    // Fill tail window
	tailWindow[n % TAIL_WINDOW_SIZE] = adcValue;
	++n;  // oldest element in the window is now at n % TAIL_WINDOW_SIZE

	// Calculate tail slope, if a full window has been acquired
	double slope = SLOPE_THRESHOLD + 1;
	if (n >= TAIL_WINDOW_SIZE) {
		slope = linearRegressionSlopeEquidistant(timestep, tailWindow, TAIL_WINDOW_SIZE, n);
		slope = fabs(slope);
	}

	double duration = -1.0;
	if (slope < SLOPE_THRESHOLD) {
		duration = (n - TAIL_WINDOW_SIZE) * timestep;
	}

	return duration;
}
