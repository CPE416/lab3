#include <stdio.h>

#include "defs.h"
#include "neural.h"
#include "net_analysis.h"
#include "line_data_iterator.h"
#include "line_follow_pid.h"


#define LEARNING_RATE (0.0001)

#define MAX_EPOCHS 2000

#define ITER_TRAINING_RATE (2)


int main(void)
{
    line_data_t line_data;
    init_line_data_iter(ITER_TRAINING_RATE);

    motor_command_t motors;

    neural_net_t net;
    init_net(&net, LEARNING_RATE);
    
    net_outputs_t outputs;    
    for(int i = 0; i<1000; i++){
        line_data.left = rand()/RAND_MAX;
        line_data.right = 150;

        motors = compute_proportional(line_data.left, line_data.right);

        print_net(net);
        infer_net(line_data, net, &outputs);
        print_outputs(outputs);
        print_results(line_data, outputs.output, motors);

    
        train_net(line_data, &net, motors);
    
        print_net(net);
        infer_net(line_data, net, &outputs);
        print_outputs(outputs);
        print_results(line_data, outputs.output, motors);
    }
    

    for(int epoch = 0; epoch < MAX_EPOCHS; epoch++){
        while (continue_epoch()){
            line_data = get_line_iter();
            motors = compute_proportional(line_data.left, line_data.right);
            infer_net(line_data, net, &outputs);
            
            print_results(line_data, outputs.output, motors);

            train_net(line_data, &net, motors);
        }
        print_net(net);
    }
    return 0;
}
