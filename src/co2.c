#include "co2.h"
#include <mh_z19.h>

int measureCo2() {
    // The parameter is the USART port the MH-Z19 sensor is connected to - in this case USART3
    mh_z19_initialise(ser_USART3);

    uint16_t ppm;
    mh_z19_returnCode_t rc;
    rc = mh_z19_takeMeassuring();
    if (rc != MHZ19_OK)
    {
        // Something went wrong
        return -1;
    }

    return ppm;
}