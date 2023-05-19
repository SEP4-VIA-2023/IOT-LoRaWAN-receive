#include <stdio.h>
#include <stdbool.h>
#include <avr/delay.h>
#include <hih8120.h>
#include "humidity.h"


/**
 * Initializes the humidity sensor and checks if the initialization was successful.
 * Prints a message indicating the status of the driver.
 */
void initialiseHumidity()
{
    // Call the humidity sensor initialization function
    hih8120_driverReturnCode_t returncode = hih8120_initialise();

    // Check if the initialization was successful
    if (returncode == HIH8120_OK)
    {
        printf("Humidity driver has been initialized\n");
    }
    else
    {
        printf("Driver not working\n");
    }
}

/**
 * This function performs the measurement of humidity.
 * It waits for the measure event to be triggered, wakes up the sensor, delays for the required startup time, measures the humidity, and fetches the data.
 * If the measurement is not successful, it retries a fixed number of times.
 * If the result is OK, it updates the _lastMeasurementHumidity variable and sets the new data event.
 */
void measureHumidity()
{
    
}

/**
 * This is the task function that continuously calls the measureHumidity() function to perform humidity measurements.
 */
void humiditySensorTask(void *pvParameters)
{
  while (1)
  {
    uint16_t Humidity_Percentage = 0;
    if (xSemaphoreTake(humiditySemaphore,portMAX_DELAY) == pdTRUE)
    {
        measureHumidity();
        Humidity_Percentage = hih8120_getHumidityPercent_x10();
        printf("Humidity: %d\n", Humidity_Percentage);

    }
    else
    {
        Humidity_Percentage = CONFIG_INVALID_HUMIDITY_VALUE;
    }
    xQueueSendToBack(sensorDataQueue, &Humidity_Percentage, portMAX_DELAY);
    xEventGroupSetBits(dataEventGroup,BIT_HUMIDITY);
  }
  
}

/**
 * This function returns the last measured humidity value.
 */
void createHumidityTask (UBaseType_t TaskPriority)
{
    
}


