#include "humidity.h"
#include <avr/delay.h>
#include <hih8120.h>

int measureHumidity() {
    if ( HIH8120_OK == hih8120_initialise() )
    {
        // Driver initialised OK
        // Always check what hih8120_initialise() returns
    }

    float humidity = 0.0;
    //float temperature = 0.0;

    if ( HIH8120_OK != hih8120_wakeup() )
    {
        // Something went wrong
        // Investigate the return code further
        return -1;
    }
    _delay_ms(100);

    if ( HIH8120_OK !=  hih8120_measure() )
    {
        // Something went wrong
        // Investigate the return code further
        return -1;
    }

    _delay_ms(4);

    humidity = hih8120_getHumidity();
    //temperature = hih8120_getTemperature();
    return humidity;
}