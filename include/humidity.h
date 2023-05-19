#ifndef HUMIDITY_H
#define HUMIDITY_H

#include <hih8120.h>

// FreeRTOS
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>
#include <event_groups.h>

// Humidity Value
extern uint16_t Humidity_Percentage;

// Semaphore and Queue
extern SemaphoreHandle_t humiditySemaphore;
extern QueueHandle_t sensorDataQueue;
extern EventGroupHandle_t dataEventGroup;

// Bit positions for events
#define BIT_MEASURE   (1 << 0)
/**
 * BIT_MEASURE: Represents the event of a humidity measurement being triggered.
 * It is set in the dataEventGroup when a measurement is performed in the humiditySensorTask function.
 * Other tasks can wait for this event to know when a measurement has been performed.
*/
#define BIT_HUMIDITY  (1 << 1)
/**
 * BIT_HUMIDITY: Represents the event of new humidity data being available.
 * It is set in the dataEventGroup when new humidity data is obtained in the humiditySensorTask function.
 * Other tasks can wait for this event to know when new humidity data is ready for processing.
*/

/**
 * Initializes the humidity sensor and checks if the initialization was successful.
 * Prints a message indicating the status of the driver.
 */
void initialiseHumidity();

/**
 * This function performs the measurement of humidity.
 * It waits for the measure event to be triggered, wakes up the sensor, delays for the required startup time, measures the humidity, and fetches the data.
 * If the measurement is not successful, it retries a fixed number of times.
 * If the result is OK, it updates the Humidity_Percentage variable and sets the new data event.
 */
void measureHumidity();

/**
 * This is the task function that continuously calls the measureHumidity() function to perform humidity measurements.
 */
void humiditySensorTask(void *pvParameters);

/**
 * This function initializes the humidity sensor task and related resources.
 * It sets up the variables, creates the task, and initializes the driver.
 * 
 * @param TaskPriority The priority of the humidity sensor task.
 */
void createHumidityTask(UBaseType_t TaskPriority);

#endif /* HUMIDITY_H */
