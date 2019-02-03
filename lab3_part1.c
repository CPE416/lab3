// Name:  Jared Peter-Contesse & Spencer Shaw
// Lab 3 Part 1
// Description: 

#include <stdio.h>

// Custum delay lib
#include "delay.h"
#include "hardware.h"
#include "line_follow_pid.h"
#include "prop.h"
#include "neural.h"


// Settings
#define DELAY_MS 100 // Delay time for loop

int main(void)
{
    line_sensor_data line_data;
    motor_command motors;

    init();
    set_motors(0, 0);

    while (1)   
    {
    	line_data = read_line_sensor();
        motors = compute_proportional(line_data.left, line_data.right);
        set_motors(motors.left, motors.right);

        print_4(line_data.left, motors.left, line_data.right, motors.right);
        delay_ms(DELAY_MS);
    }

    return 27;
}
