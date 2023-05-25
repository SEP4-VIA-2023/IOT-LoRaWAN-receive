/*
 * servo.h
 *
 * Created: 23/05/2023 11:21:13
 *  Author: andrejs
 */ 


#pragma once

#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>

// Configuration to run the servo, degreeRotation should be from -100 to 100
struct Configuration {
	uint16_t minCO2_config;
	uint16_t maxCO2_config;
	int16_t minTemperature_config;
	int16_t maxTemperature_config;
	uint16_t minHumidity_config;
	uint16_t maxHumidity_config;
	int8_t degreeRotation;
};

// The status of the servo including what values it has read
struct Status {
	int8_t servoDegrees;
	uint16_t CO2_value;
	uint16_t humidity_value;
	int16_t temperature_value;
};

// must be called before FreeRTOS vTaskStartScheduler()
void initialiseServo(UBaseType_t servoTaskPriority,
					uint16_t minCO2, uint16_t maxCO2,
					int16_t minTemperature, int16_t maxTemperature,
					uint16_t minHumidity, uint16_t maxHumidity,
					int8_t degreeRotation
					);

// Returns a pointer to the status of the servo including CO2, Humidity and Temperature readings
struct Status* readStatus();

// Returns a pointer to the current configuration that the servo is using
struct Configuration* readConfiguration();

// Set new configuration
void updateConfiguration(
	uint16_t minCO2, uint16_t maxCO2,
	int16_t minTemperature, int16_t maxTemperature,
	uint16_t minHumidity, uint16_t maxHumidity,
	int8_t degreeRotation
);
