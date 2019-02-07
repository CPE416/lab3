#include <stdio.h>

#include "defs.h"
#include "neural.h"
#include "net_analysis.h"
#include "line_data_iterator.h"

// #include "line_follow_pid.h"

// Settings
#define DELAY_MS 100 // Delay time for loop

#define TRUE 1 
#define FALSE 0

#define MAX_EPOCHS 5000

int main(void)
{
    // const float delta = 1.0 / 255.0;
    motor_command_t motors;

    neural_net_t net;
    init_net(&net);
    net_outputs_t outputs;

    line_data_t line_data;
    init_line_data_iter();

    for(int epoch = 0; epoch < MAX_EPOCHS; epoch++){
        while (continue_epoch()){
                line_data = get_line_iter();
                motors = compute_proportional(line_data.left, line_data.right);
                infer_net(line_data, net, &outputs);
                train_net(line_data, &net, motors);
                increment_iter();
            }
                // print_net(net);
        chart_error(epoch, line_data, outputs);
    }
    return 0;
}
