#include <stdio.h>

#include "defs.h"
#include "neural.h"
#include "net_analysis.h"
#include "line_data_iterator.h"
#include "line_follow_pid.h"


#define DELAY_MS 100 // Delay time for loop

#define TRUE 1 
#define FALSE 0


#define LEARNING_RATE (0.01)

#define MAX_EPOCHS 8


int main(void)
{
    line_data_t line_data;
    init_line_data_iter(5);

    motor_command_t motors;

    neural_net_t net;
    init_net(&net, LEARNING_RATE);
    net_outputs_t outputs;

    print_net(net);
    

    line_data.left = 200;
    line_data.right = 150;

    motors = compute_proportional(line_data.left, line_data.right);
    infer_net(line_data, net, &outputs);

    print_results(line_data, outputs.output, motors);
    print_outputs(outputs);

    print_net(net);
    train_net(line_data, &net, motors);
    print_net(net);
    print_results(line_data, outputs.output, motors);

    // for(int epoch = 0; epoch < MAX_EPOCHS; epoch++){
    //     while (continue_epoch()){
    //         line_data = get_line_iter();
    //         motors = compute_proportional(line_data.left, line_data.right);
    //         infer_net(line_data, net, &outputs);
            
    //         // print_results(line_data, outputs.output, motors);

    //         train_net(line_data, &net, motors);
    //     }
    //     print_net(net);
    // }
    return 0;
}
