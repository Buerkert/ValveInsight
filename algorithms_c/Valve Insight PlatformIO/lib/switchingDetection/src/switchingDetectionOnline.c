#include "switchingDetectionOnline.h"


#include <math.h>
#include <string.h>

#define TAIL_WINDOW_SIZE          200

static size_t n;
static float integral = 0;
static int32_t valuesSum = 0;
static int32_t tailSum = 0;
static int16_t tailWindow[TAIL_WINDOW_SIZE];


float round2(float x) {
    return round(x * 100.0) / 100.0;
}

void switchingDetectionOnline_Reset(void) {
	n = 0;
	integral = 0;
	valuesSum = 0;
	tailSum = 0;
	memset(tailWindow, 0, sizeof(tailWindow));
}

float switchingDetectionOnline_Calculate(int16_t adcValue, float timestep) {
	// running integral
	valuesSum += adcValue;
	
	// Calculate tail mean
	// remove oldest value from sum
	tailSum -= tailWindow[n % TAIL_WINDOW_SIZE];
	tailWindow[n % TAIL_WINDOW_SIZE] = adcValue;
	// add new value to sum
	tailSum += adcValue;
	++n;

	if (n <= TAIL_WINDOW_SIZE) {
    // Not a full tail window acquired yet. Stop here.
    return 0;
  }

	float tailMean = (float)tailSum / TAIL_WINDOW_SIZE;

	float maxIntegral = n * timestep;
	float currIntegral = maxIntegral - maxIntegral - (float)valuesSum * timestep / tailMean;
	currIntegral = fabs(currIntegral);
	currIntegral = round2(currIntegral);

	return currIntegral;
}
