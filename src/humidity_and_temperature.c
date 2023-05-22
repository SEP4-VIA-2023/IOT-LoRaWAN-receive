#include "humidity_and_temperature.h"
#include <hih8120.h>

// FreeRTOS
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

// Humidity and Temperature Value
uint16_t Humidity_Percentage;
int16_t Temperature;

// Semaphore and Queue
SemaphoreHandle_t sensorSemaphore;
QueueHandle_t sensorDataQueue;
EventGroupHandle_t dataEventGroup;
void humiditySensorTaskHandle(void *pvParameters);

/**
 * Initializes the humidity  and temperature sensor and checks if the initialization was successful.
 * Prints a message indicating the status of the driver.
 */
void initialiseTask(UBaseType_t TaskPriority)
{
    // Initialize variables
    Humidity_Percentage = 0;
    Temperature = 0;

    // Create semaphore, queue, and event group
    sensorDataQueue = xQueueCreate(10, sizeof(uint16_t));
    dataEventGroup = xEventGroupCreate();

    // Call the  sensor initialization function
    hih8120_driverReturnCode_t returncode = hih8120_initialise();


    // Task handler
    //TaskHandle_t humiditySensorTaskHandle = NULL;

    // Task creation
    xTaskCreate(humiditySensorTaskHandle,
                "HumiditySensorTask",
                configMINIMAL_STACK_SIZE,
                NULL,
                TaskPriority,
                NULL);
    
    // Check if the initialization was successful
    if (returncode == HIH8120_OK)
    {
        printf("Driver has been initialized\n");
    }
    else
    {
        printf("Driver not working\n");
    }
}

/**
 * This function performs the measurement of humidity and temperature.
 * It waits for the measure event to be triggered, wakes up the sensor, delays for the required startup time, measures the humidity and temperature, and fetches the data.
 * If the measurement is not successful, it retries a fixed number of times.
 * If the result is OK, it updates the Humidity_Percentage and Temperature variable and sets the new data event.
 */
void TakeMeasurements()
{
    // Wait for the measure event to be triggered
    xEventGroupWaitBits(dataEventGroup, BIT_MEASURE, pdTRUE, pdTRUE, portMAX_DELAY);

    hih8120_driverReturnCode_t result;

    // Wake up the sensor from standby
    result = hih8120_wakeup();

    // After the sensor wakes up, it requires at least 15 ms to start measuring
    vTaskDelay(pdMS_TO_TICKS(100));

    // Perform the measurement
    result = hih8120_measure();

    // Requires a delay to fetch data from the sensor
    vTaskDelay(pdMS_TO_TICKS(20));

    if (result != HIH8120_OK)
    {
        // Retry a fixed number of times to get measurements from the sensor
        int count = 15;
        while ((result == HIH8120_TWI_BUSY) && count > 0)
        {
            result = hih8120_measure();
            vTaskDelay(pdMS_TO_TICKS(20));
            count--;
        }
    }

    if (result == HIH8120_OK)
    {
        Humidity_Percentage = hih8120_getHumidityPercent_x10();
        Temperature = hih8120_getTemperature_x10();
        xEventGroupSetBits(dataEventGroup, BIT_TEMPERATURE);
        xEventGroupSetBits(dataEventGroup, BIT_HUMIDITY);
    }
}

uint16_t ReadHumidity(){
    return Humidity_Percentage;
}

int16_t ReadTemperature(){
    return Temperature;
}

/**
 * This is the task function that continuously calls the TakeMeasurements() function to perform humidity measurements.
 */
void sensorTaskHandle(void *pvParameters)
{
    for (;;)
    {
        if (xSemaphoreTake(sensorSemaphore, portMAX_DELAY) == pdTRUE)
        {
            TakeMeasurements();
            printf("Humidity: %d\n", Humidity_Percentage);
            printf("Temperature: %d\n", Temperature);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

