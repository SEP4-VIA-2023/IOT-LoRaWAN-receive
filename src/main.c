#include <stddef.h>
#include <drivers/display_7seg.h>
#include <avr/io.h>

#include "mult.h"
#include "co2.h"
#include "humidity.h"
#include "temperature.h"
  
// main function -
// where the execution of program begins
int main()
{
    // initialises display
    display_7seg_initialise(NULL);
    // we ready to show some stuff
    display_7seg_powerUp();
    display_7seg_displayHex("BA5ED");
  
    return 0;
}