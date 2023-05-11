#include <stddef.h>
#include <avr/io.h>
#include <display_7seg.h>

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
    display_7seg_displayHex("C0DE");
  
    return 0;
}