/*
 * servo.c
 *
 * Created: 23/05/2023 11:30:08
 *  Author: andrejs
 */ 

#include "servo.h"
#include "co2.h"
#include "humidity_and_temperature.h"
#include <task.h>
#include <semphr.h>
#include <rc_servo.h>
#include <stdlib.h>

// private stuff
void servoUpdateLoop(void *pvParameters);
int servoActive;

uint16_t minCO2_config;
uint16_t maxCO2_config;
int16_t minTemperature_config;
int16_t maxTemperature_config;
uint16_t minHumidity_config;
uint16_t maxHumidity_config;

void initialiseServo(UBaseType_t servoTaskPriority,
	uint16_t minCO2, uint16_t maxCO2,
	int16_t minTemperature, int16_t maxTemperature,
	uint16_t minHumidity, uint16_t maxHumidity
) {
	minCO2_config = minCO2;
	maxCO2_config = maxCO2;
	minTemperature_config = minTemperature;
	maxTemperature_config = maxTemperature;
	minHumidity_config = minHumidity;
	maxHumidity_config = maxHumidity;
	servoActive = 0;
	
	rc_servo_initialise();
	rc_servo_setPosition(1, 0);

	
	xTaskCreate(
	servoUpdateLoop
	,  "ServoActionUpdate"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+200  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  servoTaskPriority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

void updateConfiguration(uint16_t minCO2, uint16_t maxCO2,
int16_t minTemperature, int16_t maxTemperature,
uint16_t minHumidity, uint16_t maxHumidity
) {
	minCO2_config = minCO2;
	maxCO2_config = maxCO2;
	minTemperature_config = minTemperature;
	maxTemperature_config = maxTemperature;
	minHumidity_config = minHumidity;
	maxHumidity_config = maxHumidity;
}

// Continuously updates the servo activity
void servoUpdateLoop(void *pvParameters) {
    TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1000/portTICK_PERIOD_MS; // 1000 ms
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	for(;;) {
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		//puts("Reading CO2 value..."); // stdio functions are not reentrant - Should normally be protected by MUTEX
        
		// reading values
		uint16_t co2 = readCO2();
		uint16_t hum = ReadHumidity();
		int16_t temp = ReadTemperature();
		
		int needsAction;
		needsAction = 0;
		if (co2 > maxCO2_config || co2 < minCO2_config) {
			needsAction = 1;
		}
		if (hum > maxHumidity_config || hum < minHumidity_config) {
			needsAction = 1;
		}
		if (temp > maxTemperature_config || temp < minTemperature_config) {
			needsAction = 1;
		}
		
		// using J13 port
		if (servoActive > needsAction) { // servo is moving, but doesnt have to
			rc_servo_setPosition(1, 0);
		} else if (servoActive < needsAction) { // servo is still, but has to move
			rc_servo_setPosition(1, 100);
		}
		servoActive = needsAction;
	}
}

int readServoStatus() {
	return servoActive;
}