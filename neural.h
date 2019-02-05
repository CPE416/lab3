#ifndef NEURAL_H
#define NEURAL_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "prop.h"


#define LEARNING_RATE (0.005)

#define INPUT_NODES (2)
#define HIDDEN_NODES (3)
#define OUTPUT_NODES (2)

#define INITIAL_WEIGHT (((float) rand()) / RAND_MAX)
#define INITIAL_BIAS (((float) rand()) / RAND_MAX)

typedef struct {
    u08 size;
    float bias[INPUT_NODES];
}input_layer_t;

typedef struct {
    u08 input_size;
    u08 size;
    float weights[HIDDEN_NODES][INPUT_NODES];
    float bias[HIDDEN_NODES];
}hidden_layer_t;

typedef struct {
    u08 input_size;
    u08 size;
    float weights[OUTPUT_NODES][HIDDEN_NODES];
    float bias[OUTPUT_NODES];
}output_layer_t;

typedef struct {
    u08 input_size;
    u08 size;
    float **weights;
    float *bias;
}layer_t;

typedef struct {
    float learning_rate;
    input_layer_t input_layer;
    hidden_layer_t hidden_layer;
    output_layer_t output_layer;
}neural_net_t;

typedef struct {
    float input[INPUT_NODES];
    float hidden[HIDDEN_NODES];
    float output[OUTPUT_NODES];
}net_outputs_t;

void init_net(neural_net_t *net){
    net->learning_rate = LEARNING_RATE;
    net->input_layer.size = INPUT_NODES;
    net->hidden_layer.input_size = INPUT_NODES;
    net->hidden_layer.size = HIDDEN_NODES;
    net->output_layer.input_size = HIDDEN_NODES;
    net->output_layer.size = OUTPUT_NODES;

    net->input_layer.bias[0] = INITIAL_BIAS;
    net->input_layer.bias[1] = INITIAL_BIAS;
    for(int i = 0; i < HIDDEN_NODES; i++){
        net->hidden_layer.bias[i] = INITIAL_BIAS;
        for (int j = 0; j < INPUT_NODES; j++){
            net->hidden_layer.weights[i][j] = INITIAL_WEIGHT;
        }
    }
    for(int i = 0; i < OUTPUT_NODES; i++){
        net->output_layer.bias[i] = INITIAL_BIAS;
        for (int j = 0; j < HIDDEN_NODES; j++){
            net->output_layer.weights[i][j] = INITIAL_WEIGHT;
        }
    }
}

float sigmoid(float x){
    // http://www.ece.utep.edu/research/webfuzzy/docs/kk-thesis/kk-thesis-html/node72.html
    return 1 / (1 + exp((double) - x));
    // // Accurate approximation of sigmoid
    // return 1/(1 + pow(0.3678749025, x));
    // // Alternate, might be faster on avr
    // return (tanh(x) + 1) / 2;
    // // rectified linear
    // return x * (x > 0);
}

// Infer the output of an input layer
// input should be of size INPUT_NODES
// output should be of size INPUT_NODES
void infer_input_layer(float *input, input_layer_t layer, float *output){
    for(u08 index = 0; index < INPUT_NODES; index++){
        output[index] = input[index] - layer.bias[index];
        output[index] = sigmoid(output[index]);
    }
}

// // Infer the output of an abitrary (non-input) layer.
void infer_layer(float *input, layer_t layer, float *output){
    u08 index = 0;
        while(index < layer.size){
            input[index] = 0 - layer.bias[index];
            u08 input_index = 0;
            while(input_index < layer.input_size){
                output[index] += (input[input_index]
                                        * layer.weights[index][input_index]);
                input_index += 1;
            }
            output[index] = sigmoid(output[index]);
            index += 1;
        }
}

// Infers the output of a hidden layer
void infer_hidden_layer(float *input, hidden_layer_t layer, float *output){
    // infer_layer(input, (layer_t) layer, &address);

    u08 hidden_index = 0;
    while(hidden_index < layer.size){
        input[hidden_index] = 0 - layer.bias[hidden_index];
        u08 input_index = 0;
        while(input_index < layer.input_size){
            output[hidden_index] += (input[input_index]
                                     * layer.weights[hidden_index][input_index]);
            input_index += 1;
        }
        output[hidden_index] = sigmoid(output[hidden_index]);
        hidden_index += 1;
    }
}

// Infers the output of an output layer
void infer_output_layer(float *input, output_layer_t layer, float *output){
    u08 output_index = 0;
    while(output_index < layer.size){
        output[output_index] = 0 - layer.bias[output_index];
        u08 hidden_index = 0;
        while(hidden_index < layer.input_size){
            output[output_index] += (input[hidden_index] * layer.weights[output_index][hidden_index]);
            hidden_index += 1;
        }
        output[output_index] = sigmoid(output[output_index]);
        output_index += 1;
    }
}

// Run inference on a net
// input should be of size INPUT_NODES
void infer_net(float *input, neural_net_t net, net_outputs_t *outputs){
    infer_input_layer(input, net.input_layer, outputs->input);
    infer_hidden_layer(outputs->input, net.hidden_layer, outputs->hidden);
    infer_output_layer(outputs->hidden, net.output_layer, outputs->output);
}

// Supplies training data for neural net based on compute_proportional()
motor_command_t get_training_target(line_sensor_data_t line_data){
    return compute_proportional(line_data.left, line_data.right);
}

float calculate_error(line_sensor_data_t line_data, float *output_layer_output){
    motor_command_t motors = compute_proportional(line_data.left, line_data.right);
    float target[OUTPUT_NODES] = {motors.left / 100.0, motors.right / 100.0};
    float error = 0;
    for (u08 i = 0; i < OUTPUT_NODES; i++){
        error += pow2(target[0] - output_layer_output[i]);
    }
    return error * 0.5;
}

// Run a round of training on a neural net based on input data
void train_net(neural_net_t *net, line_sensor_data_t line_data){
    // Setup data for use in inference 
    float input[2] = {0};
    input[0] = ((float) line_data.left) / 255.0;
    input[1] = ((float) line_data.right) / 255.0;
    net_outputs_t net_outputs;

    // Run inference on the net
    infer_net(input, *net, &net_outputs);

    float error = calculate_error(line_data, net_outputs.output);

    // TODO: Complete implemention of training
    // TODO: Stuff involving the error

}

// Querries a neural net for motor commands
motor_command_t compute_neural_network(line_sensor_data_t line_data, neural_net_t net){
    // Setup data for use in inference 
    float input[2] = {0};
    input[0] = ((float) line_data.left) / 255.0;
    input[1] = ((float) line_data.right) / 255.0;


    net_outputs_t net_outputs;

    // Run inference on the net
    infer_net(input, net, &net_outputs);

    // Translate output to motor commands
    motor_command_t motor_data;
    motor_data.left = (u08)(net_outputs.output[0] * 100);
    motor_data.right = (u08)(net_outputs.output[1] * 100);

    return motor_data;

}

#endif