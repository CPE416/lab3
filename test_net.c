#include <stdio.h>

#include "defs.h"
#include "neural.h"
#include "net_analysis.h"
#include "line_data_iterator.h"

#include "line_follow_pid.h"


#define LEARNING_RATE (0.002)

#define MAX_EPOCHS (5000)

int main(void)
{
    motor_command_t motors;

    line_data_t line_data;
    init_line_data_iter(1);

    neural_net_t net;
    init_net(&net, LEARNING_RATE);

    net_outputs_t outputs;
    infer_net(line_data, net, &outputs);

    print_net(net);
    print_results(line_data, outputs.output, motors);

    for(int epoch = 0; epoch < MAX_EPOCHS; epoch++){
        while (continue_epoch()){
                line_data = get_line_iter();
                motors = compute_proportional(line_data.left, line_data.right);
                infer_net(line_data, net, &outputs);

                train_net(line_data, &net, motors);

                
        }
        if (epoch % 100 == 0){
            print_error(epoch, motors, outputs);
        } 
    }


    init_line_data_iter(10);
    while (continue_epoch()){
        line_data = get_line_iter();
        motors = compute_proportional(line_data.left, line_data.right);
        infer_net(line_data, net, &outputs);
        print_results(line_data, outputs.output, motors);
    }
    print_net(net);
    return 0;
}
