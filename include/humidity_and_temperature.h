#pragma once
#include <ATMEGA_FreeRTOS.h>

//Initialises the humidity and temperature sensor
void initialiseTEMHUM(UBaseType_t TEMHUMPriority);

//Returns the humidity value of the sensor
uint16_t ReadHumidity();
//Returns the temperature value of the sensor
int16_t ReadTemperature();
