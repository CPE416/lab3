#ifndef NET_ANALYSIS_H
#define NET_ANALYSIS_H

#include "defs.h"
#include "neural.h"

void chart_error(int iteration, motor_command_t motors, net_outputs_t outputs){
    float error = calculate_error(motors, outputs.output);
    printf("%d,  %6.5f\n", iteration, error);
}


void print_results(line_data_t line_data, float *net_out, motor_command_t motors){
    printf("Inputs: %d, %d,\tNet: %3.0f, %3.0f,\tProp: %d, %d,\tError: %5.3f%c\n", 
                       line_data.left, line_data.right,
                       100 * net_out[0], 100 * net_out[1],
                       motors.left, motors.right,
                       calculate_error(motors, net_out) * 100, '%');
}

void chart_results(line_data_t line_data, float *net_out, motor_command_t motors){
    printf("%d, %d, %3.0f, %3.0f, %d, %d, %5.3f%c\n", 
                       line_data.left, line_data.right,
                       100 * net_out[0], 100 * net_out[1],
                       motors.left, motors.right,
                       calculate_error(motors, net_out) * 100, '%');
}
void print_error(int epoch_num, motor_command_t motors, net_outputs_t outputs){
    float error = calculate_error(motors, outputs.output);
    printf("Epoch: %d, Error: %5.3f%c\n", epoch_num, error * 100, '%');
}

void print_outputs(net_outputs_t outputs){
    printf("\n input: ");
    for (int i = 0; i < INPUT_NODES; i++){
        printf("%4.3f, ", outputs.input[i]);
    }
    printf("\n hidden: ");
    for (int i = 0; i < HIDDEN_NODES; i++){
        printf("%4.3f, ", outputs.hidden[i]);
    }
    
    printf("\n output: ");
    for (int i = 0; i < OUTPUT_NODES; i++){
         printf("%6.5f, ", outputs.output[i]);
    }
    printf("\n");
}

void print_hidden_weights(hidden_layer_t layer){
    float *b = layer.bias;
    float *w = layer.weights[0];

    printf("Hidden layer:\n\tnode 0: bias: %4.3f\n\t\tweights: %4.3f, %4.3f\n", b[0], w[0], w[1]);
    w = layer.weights[1];
    printf("\tnode 1: bias: %4.3f\n\t\tweights: %4.3f, %4.3f\n", b[1], w[0], w[1]);
    w = layer.weights[2];
    printf("\tnode 2: bias: %4.3f\n\t\tweights: %4.3f, %4.3f\n", b[2], w[0], w[1]);
}

void print_output_weights(output_layer_t layer){
    float *b = layer.bias;
    float *w = layer.weights[0];
    printf("Output layer:\n\tnode 0: bias: %4.3f\n\t\tweights: %4.3f, %4.3f, %4.3f\n", b[0], w[0], w[1], w[2]);
    w = layer.weights[1];
    printf("\tnode 1: bias: %4.3f\n\t\tweights: %4.3f, %4.3f, %4.3f\n", b[1], w[0], w[1], w[2]);
}

void print_net(neural_net_t net){
    print_hidden_weights(net.hidden_layer);
    print_output_weights(net.output_layer);
}

#endif