#include <stdio.h>

#include "defs.h"
#include "neural.h"

// Settings
#define DELAY_MS 100 // Delay time for loop

#define TRUE 1 
#define FALSE 0

void print_outputs(net_outputs_t outputs);
void print_net(neural_net_t net);

int main(void)
{
    // const float delta = 1.0 / 255.0;
    line_sensor_data_t line_data;
    line_data.left = 0;
    line_data.right = 0;

    neural_net_t net;
    init_net(&net);
    net_outputs_t outputs;

    // printf("input[0], input[1], output[0], output[1]\n");

    int iterations = 8;
    while(iterations > 0){
        while (line_data.left != 255){
            while(line_data.right != 255){
                printf("Starting inference\n");
                infer_net(line_data, net, &outputs);
                printf("inputs: %d, %d, outputs: %4.3f, %4.3f\n", line_data.left, line_data.right, outputs.output[0], outputs.output[1]);
                print_net(net);
                train_net(line_data, &net);
                print_net(net);
                line_data.right += 1;
            }
            line_data.right = 0;
            line_data.left += 1;
        }
        line_data.left = 0;
    iterations--;
    }
    return 0;
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

void print_net(neural_net_t net){
    printf("hidden layer: bias: %4.3f\n", net.hidden_layer.bias);
    float *w = net.hidden_layer.weights[0];
    printf("\tnode 0: %4.3f, %4.3f\n", w[0], w[1]);
    w = net.hidden_layer.weights[1];
    printf("\tnode 1: %4.3f, %4.3f\n", w[0], w[1]);
    w = net.hidden_layer.weights[2];
    printf("\tnode 2: %4.3f, %4.3f\n", w[0], w[1]);

    printf("ouput layer: bias: %4.3f\n", net.output_layer.bias);
    w = net.output_layer.weights[0];
    printf("\tnode 0: %4.3f, %4.3f, %4.3f\n", w[0], w[1], w[2]);
    w = net.output_layer.weights[1];
    printf("\tnode 0: %4.3f, %4.3f, %4.3f\n", w[0], w[1], w[2]);
}
