#ifndef NEURAL_H
#define NEURAL_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define INPUT_NODES (2)
#define HIDDEN_NODES (3)
#define OUTPUT_NODES (2)

#define INITIAL_WEIGHT (((float) rand()) / RAND_MAX)
#define INITIAL_BIAS (((float) rand()) / RAND_MAX)

typedef struct {
    u08 size;
}input_layer_t;

typedef struct {
    u08 input_size;
    u08 size;
    float bias[HIDDEN_NODES];
    float weights[HIDDEN_NODES][INPUT_NODES];
}hidden_layer_t;

typedef struct {
    u08 input_size;
    u08 size;
    float bias[OUTPUT_NODES];
    float weights[OUTPUT_NODES][HIDDEN_NODES];
}output_layer_t;

typedef struct {
    float output_node_delta[OUTPUT_NODES];
    output_layer_t new_output_layer;
    hidden_layer_t new_hidden_layer;
}training_data_t;

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

void init_net(neural_net_t *net, float learning_rate){
    net->learning_rate = learning_rate;
    net->input_layer.size = INPUT_NODES;
    net->hidden_layer.input_size = INPUT_NODES;
    net->hidden_layer.size = HIDDEN_NODES;
    net->output_layer.input_size = HIDDEN_NODES;
    net->output_layer.size = OUTPUT_NODES;

    // net->input_layer.bias = INITIAL_BIAS;
    
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
    for(u08 index = 0; index < layer.size; index++){
        output[index] = input[index];
    }
}

// Infers the output of a hidden layer
void infer_hidden_layer(float *input, hidden_layer_t layer, float *output){
    for(u08 hidden_index = 0; hidden_index < layer.size; hidden_index++){
        input[hidden_index] = 0 - layer.bias[hidden_index];
        
        for(u08 input_index = 0; input_index < layer.input_size; input_index++){
            output[hidden_index] += (input[input_index]
                                     * layer.weights[hidden_index][input_index]);
        }
        output[hidden_index] = sigmoid(output[hidden_index]);
    }
}

// Infers the output of an output layer
void infer_output_layer(float *input, output_layer_t layer, float *output){
    for(u08 output_index = 0; output_index < layer.size; output_index++){
        output[output_index] = 0 - layer.bias[output_index];

        for (u08 hidden_index = 0; hidden_index < layer.input_size; hidden_index++){
            output[output_index] += (input[hidden_index] * layer.weights[output_index][hidden_index]);
        }
        output[output_index] = sigmoid(output[output_index]);
    }
}

// Setup data for use in inference 
void transform_input(line_data_t line_data, float *input){
    // Put line data between 0.0 and 1.0
    input[0] = ((float) line_data.left) / 255.0;
    input[1] = ((float) line_data.right) / 255.0;
    // printf("inputs: left: %9.6f, right: %9.6f\n", input[0], input[1]);
}

// Run inference on a net
// input should be of size INPUT_NODES
void infer_net(line_data_t line_data, neural_net_t net, net_outputs_t *outputs){
    float input[2];
    transform_input(line_data, input);

    infer_input_layer(input, net.input_layer, outputs->input);
    infer_hidden_layer(outputs->input, net.hidden_layer, outputs->hidden);
    infer_output_layer(outputs->hidden, net.output_layer, outputs->output);
}

// Supplies training data for neural net based on compute_proportional()
void get_training_target(motor_command_t motors, float *target){
    target[0] = motors.left / 100.0;
    target[1] = motors.right / 100.0;
}

float calculate_error(motor_command_t motors,  float output_layer_output[OUTPUT_NODES]){
    float target[OUTPUT_NODES];
    get_training_target(motors, target);

    float error = 0;
    for (u08 i = 0; i < OUTPUT_NODES; i++){
        error += pow2(target[i] - output_layer_output[i]);
    }
    return error * 0.5;
}

void copy_hidden_weights(hidden_layer_t new_layer, hidden_layer_t *dest_layer){
    // Hidden layer loop
    for(u08 hidden_index = 0; hidden_index < dest_layer->size; hidden_index ++){
        dest_layer->bias[hidden_index] = new_layer.bias[hidden_index];
        // Input layer loop
        for(u08 input_index = 0; input_index < dest_layer->input_size; input_index ++){
            dest_layer->weights[hidden_index][input_index] = new_layer.weights[hidden_index][input_index];
        }
    }
}

void copy_output_weights(output_layer_t new_layer, output_layer_t *dest_layer){
    // Output layer loop
    for(u08 output_index = 0; output_index < dest_layer->size; output_index ++){
        dest_layer->bias[output_index] = new_layer.bias[output_index];
        // Hidden layer loop
        for(u08 hidden_index = 0; hidden_index < dest_layer->input_size; hidden_index ++){
            dest_layer->weights[output_index][hidden_index] = new_layer.weights[output_index][hidden_index];
        }
    }
}

void train_output_layer(net_outputs_t outputs, neural_net_t net, float *target, training_data_t *training_data){
    // Output layer loop
    for(u08 output_index = 0; output_index < net.output_layer.size; output_index++){
        float out_min_target = outputs.output[output_index] - target[output_index];
        float out_prime = outputs.output[output_index] * (1 - outputs.output[output_index]);
        float node_delta = out_min_target * out_prime;

        training_data->output_node_delta[output_index] = node_delta;

        float old_bias = net.output_layer.bias[output_index];
        float new_bias = old_bias - (net.learning_rate * node_delta * -1.0);

        training_data->new_output_layer.bias[output_index] = new_bias;
        // Hidden layer loop
        for (u08 hidden_index = 0; hidden_index < net.output_layer.input_size; hidden_index++){
            float hidden_out = outputs.hidden[hidden_index];
            float old_weight = net.output_layer.weights[output_index][hidden_index];
            float new_weight = old_weight - (net.learning_rate * node_delta * hidden_out);

            training_data->new_output_layer.weights[output_index][hidden_index] = new_weight;
        }
    }
}

void train_hidden_layer(net_outputs_t outputs, neural_net_t net, float *target, training_data_t *training_data){
    // Hidden layer loop
    for(u08 hidden_index = 0; hidden_index < net.output_layer.size; hidden_index++){
        float dedoj = 0; // Sum of weights coming out
        // Output layer loop
        for (u08 output_index = 0; output_index < net.output_layer.size; output_index++){
            float out_node_weight = net.output_layer.weights[output_index][hidden_index];
            float out_node_delta = training_data->output_node_delta[output_index];
            dedoj += out_node_weight * out_node_delta;
        }

        // correct derivative of sigmoid hidden node output
        float dodnj = outputs.hidden[hidden_index] * (1 - outputs.hidden[hidden_index]);

        float old_bias = net.hidden_layer.bias[hidden_index];
        float new_bias = old_bias - (net.learning_rate * dedoj * dodnj * (0.0 - 1.0));

        training_data->new_hidden_layer.bias[hidden_index] = new_bias;

        // Input layer loop
        for (u08 input_index = 0; input_index < net.hidden_layer.input_size; input_index++){

            float input = outputs.input[input_index]; 
            float delta = net.learning_rate * dedoj * dodnj * input;
            
            float old_weight = net.output_layer.weights[hidden_index][input_index];

            training_data->new_hidden_layer.weights[hidden_index][input_index] = old_weight - delta;
        }
    }
}


// Run a round of training on a neural net based on input data
void train_net(line_data_t line_data, neural_net_t *net, motor_command_t motors){
    net_outputs_t net_outputs;
    infer_net(line_data, *net, &net_outputs);

    // float error = calculate_error(line_data, net_outputs.output);

    float target[OUTPUT_NODES];
    target[0] = motors.left;
    target[1] = motors.right;


    training_data_t training_data;
    
    train_output_layer(net_outputs, *net, target, &training_data);
    train_hidden_layer(net_outputs, *net, target, &training_data);

    copy_hidden_weights(training_data.new_hidden_layer, &(net->hidden_layer));
    copy_output_weights(training_data.new_output_layer, &(net->output_layer));
}

// Querries a neural net for motor commands
motor_command_t compute_neural_network(line_data_t line_data, neural_net_t net){
    net_outputs_t net_outputs;

    // Run inference on the net
    infer_net(line_data, net, &net_outputs);

    // Translate output to motor commands
    motor_command_t motor_data;
    motor_data.left = (u08)(net_outputs.output[0] * 100);
    motor_data.right = (u08)(net_outputs.output[1] * 100);

    return motor_data;
}

#endif