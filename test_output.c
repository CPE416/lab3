#include <stdio.h>

#include "defs.h"
#include "neural.h"
#include "net_analysis.h"

// #include "line_follow_pid.h"

// Settings
#define DELAY_MS 100 // Delay time for loop

#define TRUE 1 
#define FALSE 0

#define MAX_ITERATIONS 2


void print_results(line_sensor_data_t line_data, float *net_out, motor_command_t motors);

int main(void)
{
    // const float delta = 1.0 / 255.0;
    line_sensor_data_t line_data;
    line_data.left = 0;
    line_data.right = 0;

    motor_command_t motors;

    neural_net_t net;
    init_net(&net);
    net_outputs_t outputs;

    for(int iteration = 0; iteration < MAX_ITERATIONS; iteration++){
        while (line_data.left != 255){
            while(line_data.right != 255){
                infer_net(line_data, net, &outputs);
                motors = compute_proportional(line_data.left, line_data.right);
                
                print_results(line_data, outputs.output, motors);

                train_net(line_data, &net);
                line_data.right += 5;
            }
            line_data.right = 0;
            line_data.left += 5;
            print_net(net);
        }
        line_data.left = 0;
    }
    return 0;
}
void print_results(line_sensor_data_t line_data, float *net_out, motor_command_t motors){
    printf("Inputs: %d, %d, Net: %3.0f, %3.0f, Prop: %d, %d\n", 
                       line_data.left, line_data.right,
                       100 * net_out[0], 100 * net_out[1],
                       motors.left, motors.right);
}
