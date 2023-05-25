#pragma once
#include <ATMEGA_FreeRTOS.h>

// must be called before FreeRTOS vTaskStartScheduler()
void initialiseCO2(UBaseType_t CO2Priority);

// Returns the CO2 value of the sensor
uint16_t readCO2();