#include <stddef.h>
#include <avr/io.h>
#include <display_7seg.h>
#include <avr/delay.h>
  
// main function -
// where the execution of program begins



/*
* main.c
* Author : IHA
*
* Example main file including LoRaWAN setup
* Just for inspiration :)
*/

#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <serial.h>

// Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>

// including readers
#include "co2.h"
#include "humidity_and_temperature.h"
#include "servo.h"

// define two Tasks
void task1( void *pvParameters );
void task2( void *pvParameters );

// define semaphore handle
SemaphoreHandle_t xTestSemaphore;

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

/*-----------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if ( xTestSemaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		xTestSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		if ( ( xTestSemaphore ) != NULL )
		{
			xSemaphoreGive( ( xTestSemaphore ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}

	xTaskCreate(
	task1
	,  "Task1"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );

	xTaskCreate(
	task2
	,  "Task2"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

/*-----------------------------------------------------------*/
void task1( void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 500/portTICK_PERIOD_MS; // 500 ms

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		puts("Task1"); // stdio functions are not reentrant - Should normally be protected by MUTEX
		PORTA ^= _BV(PA0);
	}
}

/*-----------------------------------------------------------*/
void task2( void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1000/portTICK_PERIOD_MS; // 1000 ms
	const TickType_t TextWait = 5000/portTICK_PERIOD_MS; // 5000 ms
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		//puts("Task2"); // stdio functions are not reentrant - Should normally be protected by MUTEX
		//PORTA ^= _BV(PA7);
		
		display_7seg_displayHex("C02");
		xTaskDelayUntil( &xLastWakeTime, TextWait );
		uint16_t co2 = readCO2();
		// we ready to show some stuff
		display_7seg_display(co2, 0);
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		
		display_7seg_displayHex("7E177");
		xTaskDelayUntil( &xLastWakeTime, TextWait );
		int16_t temp = ReadTemperature();
		// we ready to show some stuff
		display_7seg_display(temp, 0);
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		
		display_7seg_displayHex("1770157");
		xTaskDelayUntil( &xLastWakeTime, TextWait );
		uint16_t hum = ReadHumidity();
		// we ready to show some stuff
		display_7seg_display(hum, 0);
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		
	}
}

/*-----------------------------------------------------------*/
void initialiseSystem()
{
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);

	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	
	display_7seg_initialise(NULL);
	display_7seg_powerUp();
	initialiseCO2();
	intitialiseTEMHUM();
	initialiseServo(2,0,100,50,500,0,1000);
	
	
	// Let's create some tasks
	create_tasks_and_semaphores();

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_initialise(1, NULL);
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3);
}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing!!
	
	    
	printf("Program Started!!\n");
	vTaskStartScheduler(); // Initialise and run the freeRTOS scheduler. Execution should never return from here.

	/* Replace with your application code */
	while (1)
	{
	}
}

