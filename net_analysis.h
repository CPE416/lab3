#ifndef NET_ANALYSIS_H
#define NET_ANALYSIS_H

#include "defs.h"
#include "neural.h"

void chart_error(int iteration, line_sensor_data_t line_data, net_outputs_t outputs){
    float error = calculate_error(line_data, outputs.output);
    printf("%d,  %6.5f\n", iteration, error);
}

void print_error(line_sensor_data_t line_data, net_outputs_t outputs){
    float error = calculate_error(line_data, outputs.output);
    printf("Error: %4.3f\n", error);
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
    printf("\n");
    // printf("\n output: ");
    // for (int i = 0; i < OUTPUT_NODES; i++){
    //     printf("%6.5f, ", outputs.output[i]);
    // }
}

void print_hidden_weights(hidden_layer_t layer){
    float *w = layer.weights[0];
    printf("\tnode 0: %4.3f, %4.3f\n", w[0], w[1]);
    w = layer.weights[1];
    printf("\tnode 1: %4.3f, %4.3f\n", w[0], w[1]);
    w = layer.weights[2];
    printf("\tnode 2: %4.3f, %4.3f\n", w[0], w[1]);
}

void print_output_weights(output_layer_t layer){
    float *w = layer.weights[0];
    printf("\tnode 0: %4.3f, %4.3f, %4.3f\n", w[0], w[1], w[2]);
    w = layer.weights[1];
    printf("\tnode 0: %4.3f, %4.3f, %4.3f\n", w[0], w[1], w[2]);
}

void print_net(neural_net_t net){
    printf("hidden layer: bias: %4.3f\n", net.hidden_layer.bias);
    print_hidden_weights(net.hidden_layer);

    printf("output layer: bias: %4.3f\n", net.output_layer.bias);
    print_output_weights(net.output_layer);
}

#endif