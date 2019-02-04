#include <stdio.h>

#include "defs.h"
#include "neural.h"

// Settings
#define DELAY_MS 100 // Delay time for loop

#define TRUE 1 
#define FALSE 0

void print_outputs(net_outputs_t outputs);

int main(void)
{
    const float delta = 1.0 / 255.0;
    float inputs[2] = {0.0, 0.0};
    neural_net_t net;
    init_net(&net);
    net_outputs_t outputs;

    printf("input[0], input[1], output[0], output[1]\n");
    while (inputs[0] <= 1){
        while(inputs[1] <= 1){
            // printf("Starting inference\n");
            infer_net(inputs, net, &outputs);
            print_outputs(outputs);
            printf("%6.5f, %6.5f, %6.5f, %6.5f\n", inputs[0], inputs[1], outputs.output[0], outputs.output[1]);
            inputs[1] += delta;
        }
        inputs[1] = 0;
        inputs[0] += delta;
    }
    return 0;
}

void print_outputs(net_outputs_t outputs){
    printf("\n input: ");
    for (int i = 0; i < INPUT_NODES; i++){
        printf("%6.5f, ", outputs.input[i]);
    }
    printf("\n hidden: ");
    for (int i = 0; i < HIDDEN_NODES; i++){
        printf("%6.5f, ", outputs.hidden[i]);
    }
    printf("\n");
    // printf("\n output: ");
    // for (int i = 0; i < OUTPUT_NODES; i++){
    //     printf("%6.5f, ", outputs.output[i]);
    // }
}
