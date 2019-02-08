#include <stdio.h>

#include "defs.h"
#include "neural.h"
#include "net_analysis.h"
#include "line_data_iterator.h"

#include "line_follow_pid.h"


#define LEARNING_RATE (0.01)

#define MAX_EPOCHS 1

int main(void)
{
    motor_command_t motors;

    line_data_t line_data;
    init_line_data_iter();

    neural_net_t net;
    init_net(&net, LEARNING_RATE);
    print_net(net);
    net_outputs_t outputs;


    infer_net(line_data, net, &outputs);
    chart_error(0, motors, outputs);
    for(int epoch = 0; epoch < MAX_EPOCHS; epoch++){
        while (continue_epoch()){
                line_data = get_line_iter();
                motors = compute_proportional(line_data.left, line_data.right);
                infer_net(line_data, net, &outputs);
                train_net(line_data, &net, motors);
            }
        chart_error(epoch, motors, outputs);
    }
    chart_error(MAX_EPOCHS, motors, outputs);
    print_net(net);
    return 0;
}
