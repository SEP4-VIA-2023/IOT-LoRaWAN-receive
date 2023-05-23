#pragma once
#include<stdio.h>

//Initialises the humidity and temperature sensor
void intitialiseTEMHUM();

//Returns the humidity value of the sensor
uint16_t ReadHumidity();
//Returns the temperature value of the sensor
int16_t ReadTemperature();
