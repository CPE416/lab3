#include <stdio.h>

#include "defs.h"
#include "neural.h"
#include "net_analysis.h"

// #include "line_follow_pid.h"

// Settings
#define DELAY_MS 100 // Delay time for loop

#define TRUE 1 
#define FALSE 0

#define MAX_ITERATIONS 5000

int main(void)
{
    // const float delta = 1.0 / 255.0;
    line_sensor_data_t line_data;
    line_data.left = 0;
    line_data.right = 0;

    neural_net_t net;
    init_net(&net);
    net_outputs_t outputs;

    for(int iteration = 0; iteration < MAX_ITERATIONS; iteration++){
        // print_net(net);
        while (line_data.left != 255){
            while(line_data.right != 255){
                infer_net(line_data, net, &outputs);
                train_net(line_data, &net);

                line_data.right += 5;
            }
                // print_net(net);
            line_data.right = 0;
            line_data.left += 5;
        }
        line_data.left = 0;
        chart_error(iteration, line_data, outputs);
    }
    return 0;
}
