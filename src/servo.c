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

// private stuff
void servoUpdateLoop(void *pvParameters);

// servo status including what values it has read
struct Status status;
// the current configuration the servo runs on
struct Configuration configuration;

void initialiseServo(UBaseType_t servoTaskPriority,
	uint16_t minCO2, uint16_t maxCO2,
	int16_t minTemperature, int16_t maxTemperature,
	uint16_t minHumidity, uint16_t maxHumidity,
	int8_t degreeRotation
) {
	// setting up initial status
	status.CO2_value = 0;
	status.humidity_value = 0;
	status.temperature_value = 0;
	status.servoDegrees = 0;
	
	// setting up initial configuration
	configuration.degreeRotation = degreeRotation;
	configuration.minCO2_config = minCO2;
	configuration.maxCO2_config = maxCO2;
	configuration.minHumidity_config = minHumidity;
	configuration.maxHumidity_config = maxHumidity;
	configuration.minTemperature_config = minTemperature;
	configuration.maxTemperature_config = maxTemperature;
	
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
	uint16_t minHumidity, uint16_t maxHumidity,
	int8_t degreeRotation
) {
	configuration.degreeRotation = degreeRotation;
	configuration.minCO2_config = minCO2;
	configuration.maxCO2_config = maxCO2;
	configuration.minHumidity_config = minHumidity;
	configuration.maxHumidity_config = maxHumidity;
	configuration.minTemperature_config = minTemperature;
	configuration.maxTemperature_config = maxTemperature;
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
		status.CO2_value = readCO2();
		status.humidity_value = ReadHumidity();
		status.temperature_value = ReadTemperature();
		
		int needsAction;
		needsAction = 0;
		if (status.CO2_value > configuration.maxCO2_config ||
			status.CO2_value < configuration.minCO2_config) {
			needsAction = 1;
		}
		if (status.humidity_value > configuration.maxHumidity_config ||
			status.humidity_value < configuration.minHumidity_config) {
			needsAction = 1;
		}
		if (status.temperature_value > configuration.maxTemperature_config ||
			status.temperature_value < configuration.minTemperature_config) {
			needsAction = 1;
		}
		
		// using J13 port
		if (needsAction) {
			// servo needs to move
			if (status.servoDegrees != configuration.degreeRotation) {
				// servo hasn't moved yet
				rc_servo_setPosition(1, configuration.degreeRotation);
				status.servoDegrees = configuration.degreeRotation;
			}
		} else {
			// servo should be idle
			if (status.servoDegrees != 0) {
				// servo isn't idle, resetting position
				rc_servo_setPosition(1, 0);
				status.servoDegrees = 0;
			}
		}
	}
}

struct Status* readStatus() {
	return &status;
}

struct Configuration* readConfiguration() {
	return &configuration;
}