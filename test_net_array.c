#include <stdio.h>

#include "defs.h"
#include "neural.h"
#include "net_analysis.h"
#include "line_data_iterator.h"

#include "line_follow_pid.h"

#define NUM_NETS 4
#define MAX_EPOCHS (1000)

int main(void)
{
    motor_command_t motors;

    line_data_t line_data;
    init_line_data_iter(1);

    float learning_rate[NUM_NETS] = {0.003, 0.002, 0.001, 0.0005};
    neural_net_t nets[NUM_NETS];
    net_outputs_t outputs;

    for (int i = 0; i < NUM_NETS; i++){
        init_net(&(nets[i]), learning_rate[i]);

        infer_net(line_data, nets[i], &outputs);

        // print_net(net);
        // print_results(line_data, outputs.output, motors);
        printf("Net: %d, learning rate: %5.4f\n", i, nets[i].learning_rate);
        print_error(0, motors, outputs);
    }
    for(int epoch = 0; epoch < MAX_EPOCHS; epoch++){
        for (int i = 0; i < NUM_NETS; i++){
            while (continue_epoch()){
                    line_data = get_line_iter();
                    motors = compute_proportional(line_data.left, line_data.right);
                    infer_net(line_data, nets[i], &outputs);

                    train_net(line_data, &(nets[i]), motors);  
            }
        }
    }
    for (int i = 0; i < NUM_NETS; i++){
        // print_net(nets[i]);
        infer_net(line_data, nets[i], &outputs);
        // print_results(line_data, outputs.output, motors);
        printf("Net: %d, learning rate: %5.4f\n", i, nets[i].learning_rate);
        print_error(MAX_EPOCHS, motors, outputs);
    }
    return 0;
}
