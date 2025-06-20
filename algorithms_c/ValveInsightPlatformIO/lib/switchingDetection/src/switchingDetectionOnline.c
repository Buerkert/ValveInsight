#include "switchingDetectionOnline.h"


#include <math.h>
#include <string.h>

#define TAIL_WINDOW_SIZE          200

static size_t n;
static int32_t valuesSum = 0;
static int32_t tailSum = 0;
static int16_t tailWindow[TAIL_WINDOW_SIZE];


float round2(float x) {
    return round(x * 100.0) / 100.0;
}

void switchingDetectionOnline_Reset(void) {
	n = 0;
	valuesSum = 0;
	tailSum = 0;
	memset(tailWindow, 0, sizeof(tailWindow));
}

float switchingDetectionOnline_Calculate(int16_t adcValue, float timestep) {
	// running integral
	valuesSum += adcValue;
	
	if (n < TAIL_WINDOW_SIZE) {
        tailSum += adcValue;
        tailWindow[n] = adcValue;
        ++n;
        // Not a full tail window acquired yet. Stop here.
        return 0;
    } 

    // Calculate tail mean
    size_t idx = n % TAIL_WINDOW_SIZE;
    tailSum -= tailWindow[idx];  // remove oldest value from sum
	tailSum += adcValue;  // add new value to sum
    tailWindow[idx] = adcValue;  // Update the tail window with the new value
	++n;

    float tailMean = (float)tailSum / TAIL_WINDOW_SIZE;
	float currIntegral = timestep * (n - (float)valuesSum / tailMean);

	currIntegral = fabs(currIntegral);
	currIntegral = round2(currIntegral);

	return currIntegral;
}
