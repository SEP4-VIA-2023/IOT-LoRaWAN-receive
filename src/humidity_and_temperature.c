#include "humidity_and_temperature.h"
#include <hih8120.h>

// FreeRTOS
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

//Values
//HUM = Humidity
uint16_t HUM;
//TEM = Temperature
int16_t TEM;

void TEMHUMReadingLoop(void *pvParameters);

void intitialiseTEMHUM(UBaseType_t TEMHUMPriority){
    hih8120_initialise();
    hih8120_wakeup();
    
    // Initialize variables
    HUM = 0;
    TEM = 0;

	//TEM and HUM values from the reading loop
	xTaskCreate(
	TEMHUMReadingLoop
	, "TEMHUMReadingLoop"
	, configMINIMAL_STACK_SIZE
	, NULL
	, TEMHUMPriority
	, NULL
	);

}

void TEMHUMMeasure()
{
 

    if ( HIH8120_OK != hih8120_wakeup() )
    {
        puts("Failed waking up the sensor");
    }
    vTaskDelay(pdMS_TO_TICKS(100));

    if ( HIH8120_OK !=  hih8120_measure() )
    {
        puts("Failed measuring data");
    }

    vTaskDelay(pdMS_TO_TICKS(20));
}

//Reads temperature and humidity data continuously
void TEMHUMReadingLoop(void *pvParameters)
{
    TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1000/portTICK_PERIOD_MS; // 1000 ms
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

    for(;;){
    xTaskDelayUntil( &xLastWakeTime, xFrequency );
	puts("Reading Temperature and Humidity value...");
    TEMHUMMeasure();
    TEM =hih8120_getTemperature_x10();
    HUM = hih8120_getHumidityPercent_x10();
    }   
}

uint16_t ReadHumidity()
{
    return HUM;
}
int16_t ReadTemperature()
{
    return TEM;
}
