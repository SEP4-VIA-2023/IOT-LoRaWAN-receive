#include <stddef.h>
#include <avr/io.h>
#include <display_7seg.h>
#include <avr/delay.h>

#include "co2.h"
#include "humidity.h"
#include "temperature.h"
  
// main function -
// where the execution of program begins
int main(){
    // initialises display
    display_7seg_initialise(NULL);
	_delay_ms(1000);	
    // we ready to show some stuff
    display_7seg_powerUp();
	_delay_ms(1000);
    display_7seg_display(3.14159265359, 2);
  
    return 0;
}