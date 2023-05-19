#pragma once
#include <stdio.h>

// must be called before FreeRTOS vTaskStartScheduler()
void initialiseCO2();

// Returns the CO2 value of the sensor
uint16_t readCO2();