/*
 * servo.h
 *
 * Created: 23/05/2023 11:21:13
 *  Author: andrejs
 */ 


#pragma once

#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>

// must be called before FreeRTOS vTaskStartScheduler()
void initialiseServo(UBaseType_t servoTaskPriority,
					uint16_t minCO2, uint16_t maxCO2,
					int16_t minTemperature, int16_t maxTemperature,
					uint16_t minHumidity, uint16_t maxHumidity
					);

// Returns the status of the servo (1 if it's moving)
int readServoStatus();

// Set new configuration
void updateConfiguration(
	uint16_t minCO2, uint16_t maxCO2,
	int16_t minTemperature, int16_t maxTemperature,
	uint16_t minHumidity, uint16_t maxHumidity
);
