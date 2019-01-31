// Name:  Jared Peter-Contesse & Spencer Shaw
// Lab 2 Part 1
// Description: 

#include <stdio.h>

// Custum delay lib
#include "delay.h"
#include "hardware.h"
#include "prop.h"

// Settings
#define DELAY_MS 100 // Delay time for loop
#define DELTA_INIT 4 // Braitenberg mode


int main(void)
{
    u08 light_left;
    u08 light_right;
    struct motor_command motors;

    init();
    set_motors(0, 0);

    while (1)   
    {

    	light_left = poll_analog_pin(LINE_SENSOR_LEFT);
    	light_right = poll_analog_pin(LINE_SENSOR_RIGHT);

        motors = compute_proportional(light_left, light_right)
        set_motors(motors.left, motors.right);

        print_4(light_left, motors.left, light_right, motors);
        delay_ms(DELAY_MS);
    }

    return 27;
}
