// Simple C program to display "Hello World"
  
// Header file for input output functions
#include <stdio.h>
#include "mult.h"
#include <drivers/display_7seg.h>
#include <avr/io.h>
  
// main function -
// where the execution of program begins
int main()
{
    // initialises display
    /*display_7seg_initialise(NULL);
    // we ready to show some stuff
    display_7seg_powerUp();
    display_7seg_displayHex("BA5ED");*/
    
    printf("Result: %d", multiply(5,3));
  
    return 0;
}