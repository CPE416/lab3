#include <stdio.h>

#include "defs.h"
#include "neural.h"

// Settings
#define DELAY_MS 100 // Delay time for loop

#define TRUE 1 
#define FALSE 0

line_sensor_data_t line_data;

void increment_line_data();

int main(void)
{
    // printf("Starting test\n");
    // u08 exit = FALSE;
    motor_command_t motors;
    motors.left = 255;
    motors.right = 0;

    neural_net_t net;
    init_net(&net);
    // printf("Initialized\n");
    // while (!exit){
    	increment_line_data();
        // printf("Starting inference\n");
        motors = compute_neural_network(line_data, net);
        // printf("Line: %d, %d\tmotors: %d, %d\n", line_data.left, line_data.right, motors.left, motors.right);
        // exit = TRUE;
    // }
    return 0;
}

void increment_line_data(){
    if(line_data.left == 255){
        line_data.right += 1;
    }
    line_data.left += 1;
    // printf("Line data: %d, %d\n", line_data.left, line_data.right);
}
