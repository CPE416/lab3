#include <stdio.h>

#include "defs.h"
#include "neural.h"
#include "net_analysis.h"
#include "line_data_iterator.h"

#include "line_follow_pid.h"


#define LEARNING_RATE (0.001)

#define MAX_EPOCHS (0)
#define EPOCH_PRINT_INTERVAL (MAX_EPOCHS / 10)
#define ITER_TRAINING_RATE (10)
#define ITER_PRINTING_RATE (50)

#define INFERENCE_ITERATIONS (1000)

int main()
{
    // motor_command_t _motors;

    // Create line data for input, init iterator
    line_data_t line_data;
    line_data.left = 127;
    line_data.right = 252;

    // Create and init network
    neural_net_t *net = (neural_net_t *) malloc(sizeof(neural_net_t));
    init_net(net, LEARNING_RATE);

    // Create outputs and 
    net_outputs_t outputs;
    for (int i = 0; i < INFERENCE_ITERATIONS; i++){
        infer_net(line_data, *net, &outputs);
    }

    for (int i = 0; i < HIDDEN_NODES; i++){
        printf("Output %d: %8.7f\n", i, outputs.output[i]);
    }
    // print_net(net);
    // print_results(line_data, outputs.output, motors);

    // for(int epoch = 0; epoch < MAX_EPOCHS; epoch++){
    //     while (continue_epoch()){
    //             line_data = get_line_iter();
    //             motors = compute_proportional(line_data.left, line_data.right);
    //             infer_net(line_data, net, &outputs);

    //             train_net(line_data, &net, motors);
    //     }
    //     if (epoch % EPOCH_PRINT_INTERVAL == 0){
    //         print_error(epoch, motors, outputs);
    //     } 
    // }

    // init_line_data_iter(ITER_PRINTING_RATE);
    // while (continue_epoch()){
    //     line_data = get_line_iter();
    //     motors = compute_proportional(line_data.left, line_data.right);
    //     infer_net(line_data, net, &outputs);
    //     chart_results(line_data, outputs.output, motors);
    // }
    // print_net(net);
    return 0;
}
