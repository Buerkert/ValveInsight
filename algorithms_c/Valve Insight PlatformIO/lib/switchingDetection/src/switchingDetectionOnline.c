#include "switchingDetectionFixed.h"


#include <math.h>
#include <string.h>

#define TAIL_WINDOW_SIZE          200

static size_t n;
static double integral = 0;
static double tailWindow[TAIL_WINDOW_SIZE];


double round2(double x) {
    return round(x * 100.0) / 100.0;
}

void switchingDetectionOnline_Reset(void) {
	n = 0;
	integral = 0;
	memset(tailWindow, 0, sizeof(tailWindow));
}

double switchingDetectionOnline_Calculate(int16_t adcValue, double timestep) {
	// running integral
	integral += adcValue*timestep;
	
	tailWindow[n % TAIL_WINDOW_SIZE] = adcValue;
	++n;

	// Calculate tail mean
	double tailMean = 0;
	if (n > TAIL_WINDOW_SIZE) {
		uint16_t i;
		for (i=0; i < TAIL_WINDOW_SIZE; ++i) {
			tailMean += tailWindow[i];
		}
		tailMean /= TAIL_WINDOW_SIZE;
	}else{
		// Not a full tail window acquired yet. Stop here.
		return 0;
	}

	double maxIntegral = n * timestep;
	double currIntegral = maxIntegral - integral / tailMean;
	currIntegral = fabs(currIntegral);
	currIntegral = round2(currIntegral);

	return currIntegral;
}
