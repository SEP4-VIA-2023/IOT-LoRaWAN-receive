#ifndef HUMIDITY_H
#define HUMIDITY_H

#include <stdio.h>
#include <hih8120.h>

// FreeRTOS
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>
#include <event_groups.h>


// Bit positions for events
/**
 * BIT_MEASURE: Represents the event of a humidity and temperature measurement being triggered.
 * It is set in the dataEventGroup when a measurement is performed in the sensorTaskHandle function.
 * Other tasks can wait for this event to know when a measurement has been performed.
*/
#define BIT_MEASURE   (1 << 0)

/**
 * BIT_HUMIDITY: Represents the event of new humidity data being available.
 * It is set in the dataEventGroup when new humidity data is obtained in the sensorTaskHandle function.
 * Other tasks can wait for this event to know when new humidity data is ready for processing.
*/
#define BIT_HUMIDITY  (1 << 1)
/**
 * BIT_TEMPERATURE: Represents the event of new temperaturw data being available.
 * It is set in the dataEventGroup when new temperature data is obtained in the sensorTaskHandle function.
 * Other tasks can wait for this event to know when new temperature data is ready for processing.
*/
#define BIT_TEMPERATURE (1 << 1)

/**
 * Initializes the humidity and temperature sensor and checks if the initialization was successful.
 * Prints a message indicating the status of the driver.
 * @param TaskPriority The priority of the humidity and temperature sensor task.
 */
void initialiseTask(UBaseType_t TaskPriority);

/**
 * This function performs the measurement of humidity.
 * It waits for the measure event to be triggered, wakes up the sensor, delays for the required startup time, measures the humidity, and fetches the data.
 * If the measurement is not successful, it retries a fixed number of times.
 * If the result is OK, it updates the Humidity_Percentage variable and sets the new data event.
 */
//void measureHumidity();

/**
 * This is the task function that continuously calls the measureHumidity() function to perform humidity measurements.
 */
//void humiditySensorTask(void *pvParameters);

uint16_t ReadHumidity();
int16_t ReadTemperature();

#endif /* HUMIDITY_H */
