// Include files from the Valve Insight C++ algorithms library and mbed library
#include "switchingDetectionFixed.h"
#include "mbed.h"
// #include <stdio.h>
// Change TIMESTEP for comparability with other code applications
// For simple testing, default value = 1 is okay
#define TIMESTEP 1

// Declare global variables
float analog_input_0_value;
int16_t current;
double integral_value;
char buffer[32];
UnbufferedSerial pc(USBTX, USBRX, 460800);

int main() { 
  // Initialize the peripherals
  DigitalOut output_pin(D2);
  output_pin = 0;
  AnalogIn analog_input_0(A0);
  //AnalogIn analog_input_1(A1);

  
  // Switching the valve, Collecting data, and calculating the integral value
  while (true) {
    switchingDetectionFixed_Reset();
    ThisThread::sleep_for(1000ms);
    output_pin = 1; 
    for(int i=0; i<500; i++){
        analog_input_0_value = analog_input_0.read();
        current = (int16_t)(analog_input_0_value * 32767);
        switchingDetectionFixed_StoreADC(current);
    }
    integral_value = switchingDetectionFixed_Calculate(TIMESTEP);
    snprintf(buffer, sizeof(buffer), "integral_value: %.2f\r\n", integral_value);
    pc.write(buffer, strlen(buffer));
    ThisThread::sleep_for(1000ms);
    output_pin = 0;
    }
  return 0; 
} 
