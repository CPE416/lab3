#include <stdio.h>

#include "defs.h"
#include "neural.h"
#include "net_analysis.h"
#include "line_data_iterator.h"

#include "line_follow_pid.h"


#define LEARNING_RATE (0.01)

#define MAX_EPOCHS (100)
#define EPOCH_PRINT_INTERVAL (1)
#define ITER_TRAINING_RATE (1)
#define ITER_PRINTING_RATE (25)


int main()
{
    motor_command_t motors;

    // Create line data for input, init iterator
    line_data_t line_data;
    init_line_data_iter(ITER_TRAINING_RATE);

    // Create and init network
    neural_net_t net;
    init_net(&net, LEARNING_RATE);

    // Create outputs and 
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
        if (epoch % EPOCH_PRINT_INTERVAL == 0){
            print_error(epoch, motors, outputs);
        } 
    }

    init_line_data_iter(ITER_PRINTING_RATE);
    while (continue_epoch()){
        line_data = get_line_iter();
        motors = compute_proportional(line_data.left, line_data.right);
        infer_net(line_data, net, &outputs);
        chart_results(line_data, outputs.output, motors);
    }
    print_net(net);
    return 0;
}
