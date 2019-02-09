#include <stdio.h>

#include "defs.h"
#include "neural.h"
#include "net_analysis.h"
#include "line_data_iterator.h"
#include "line_follow_pid.h"


#define LEARNING_RATE 0.002

#define MAX_EPOCHS 10

#define ITER_TRAINING_RATE (2)


int main(void)
{
    init_line_data_iter(ITER_TRAINING_RATE);

    neural_net_t net;
    init_net(&net, LEARNING_RATE);

    int training_iteration_count = 100;

    int data_counter = 100;
    int current_data_counter = 0;
    while(current_data_counter < data_counter - 1){
        
        line_data_t line = get_line_iter();
        motor_command_t motors1 = compute_proportional(line.left, line.right);
        printf("motors: %d, %d\n", motors1.left, motors1.right);
        printf("current data counter: %d\n", current_data_counter);
        train_net(line, &net, motors1);
        
        current_data_counter++;
    }
    // delay_ms(100);
    printf("training iteration count: %d\n", training_iteration_count);
    training_iteration_count--;


    return 0;
}
