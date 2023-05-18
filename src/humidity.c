#include "humidity.h"
#include <avr/delay.h>
#include <hih8120.h>

static TaskHandle_t _HumiditySensorTaskHandle;

//private fields
static SemaphoreHandle_t _xPrintfSemaphore;
static EventGroupHandle_t _eventGroupHandleMeasure;
static EventGroupHandle_t _eventGroupHandleNewData;
static float _lastMeasurementHumidity;

// Delay constants
#define DELAY_WAKEUP 15
#define DELAY_MEASUREMENT 20

/**
 * Initializes the humidity sensor and checks if the initialization was successful.
 * Prints a message indicating the status of the driver.
 */
void initialiseHumidity()
{
    // Call the humidity sensor initialization function
    hih8120_driverReturnCode_t returncode = hih8120_initialise();

    // Check if the initialization was successful
    if (HIH8120_OK == returncode)
    {
        printf("Humidity driver has been initialized\n");
    }
    else
    {
        printf("Driver not working\n");
    }
}



void measureHumidity()
{
    float humidity = 0.0;
    // Wait until the measure event is triggered
    xEventGroupWaitBits(_eventGroupHandleMeasure,
                        pdTRUE,
                        pdTRUE,
                        portMAX_DELAY);

    hih8120_driverReturnCode_t result;

    // Wakes up the sensor from standby
    result = hih8120_wakeup();
    // After the sensor wakes up, it requires at least 100 ms to start measuring
    vTaskDelay(DELAY_WAKEUP);
    result = hih8120_measure();
    // Requires a delay to fetch data from the sensor
    vTaskDelay(DELAY_MEASUREMENT);

    if (HIH8120_OK != result)
    {
        // Set trials to get measurements from the sensor
        int count = 15;
        while ((HIH8120_TWI_BUSY == result) && count > 0)
        {
            result = hih8120_measure();
            vTaskDelay(DELAY_MEASUREMENT);
            count--;
        }
    }

    if (result == HIH8120_OK)
    {
        _lastMeasurementHumidity = hih8120_getHumidity();
        xEventGroupSetBits(_eventGroupHandleNewData);
    }
}

}

void humiditySensorTask(void* pvParameters)
{
for(;;)
{
    measureHumidity();
}
vTaskDelete(_HumiditySensorTaskHandle);
}

float humiditySensor_getHumidity()
{
	return _lastMeasurementHumidity;
}