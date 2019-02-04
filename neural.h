#ifndef NEURAL_H
#define NEURAL_H

#define INPUT_NODES 2
#define HIDDEN_NODES 3
#define OUTPUT_NODES 2

#define INPUT_LAYER_OUTPUT_EMPTY {0, 0}
#define HIDDEN_LAYER_OUTPUT_EMPTY {0, 0, 0}
#define OUTPUT_LAYER_OUTPUT_EMPTY {0, 0}

typedef struct input_layer{
    float bias[INPUT_NODES];
}input_layer_t;

typedef struct hidden_layer{
    float weights[HIDDEN_NODES][INPUT_NODES];
    float bias[HIDDEN_NODES];
}hidden_layer_t;

typedef struct output_layer{
    float weights[OUTPUT_NODES][HIDDEN_NODES];
    float bias[OUTPUT_NODES];
}output_layer_t;

typedef struct neural_net{
    input_layer_t input_layer;
    hidden_layer_t hidden_layer;
    output_layer_t output_layer;
}neural_net_t;

typedef struct input_data{
    u08 left;
    u08 right;
}input_data_t;

typedef struct net_output{
    float left;
    float right;
}net_output_t;

void infer_input_layer(input_data_t input, input_layer_t layer, float *output){
    output[0] = (((float) input.left) / 255) - layer.bias[0];
    output[1] = (((float) input.right) / 255) - layer.bias[1];

    // TODO: Finish/correct calculations on this node
}

void infer_hidden_layer(float *input, hidden_layer_t layer, float *output){
    u08 hidden_index = 0;
    while(hidden_index < HIDDEN_NODES){
        input[hidden_index] = 0 - layer.bias[hidden_index];
        u08 input_index = 0;
        while(input_index < INPUT_NODES){
            output[hidden_index] += (input[input_index]
                                     * layer.weights[hidden_index][input_index]);
            input_index += 1;
        }
        hidden_index += 1;
    }
}

void infer_output_layer(float *input, output_layer_t layer, float *output){
    u08 output_index = 0;
    while(output_index < OUTPUT_NODES){
        output[output_index] = 0 - layer.bias[output_index];
        u08 hidden_index = 0;
        while(hidden_index < HIDDEN_NODES){
            output[output_index] += (input[hidden_index] * layer.weights[output_index][hidden_index]);
            hidden_index += 1;
        }
        output_index += 1;
    }
}

void infer_net(line_sensor_data_t line_data, neural_net_t net, net_output_t *output){
    input_data_t input;
    input.left = line_data.left;
    input.right = line_data.right;

    float input_layer_output[INPUT_NODES] = INPUT_LAYER_OUTPUT_EMPTY;
    infer_input_layer(input, net.input_layer, input_layer_output);

    float hidden_layer_output[HIDDEN_NODES] = HIDDEN_LAYER_OUTPUT_EMPTY;
    infer_hidden_layer(input_layer_output, net.hidden_layer, hidden_layer_output);

    float output_layer_output[OUTPUT_NODES] = OUTPUT_LAYER_OUTPUT_EMPTY;
    infer_output_layer(hidden_layer_output, net.output_layer, output_layer_output);

    output->left = output_layer_output[0];
    output->right = output_layer_output[1];
    return;
}

neural_net_t train_net(neural_net_t net, line_sensor_data_t line_data, motor_command_t motor_data){
    
    // TODO: Implement training
    
    return net;
}

motor_command_t compute_neural(line_sensor_data_t line_data, neural_net_t net){
    net_output_t net_output;
    infer_net(line_data, net, &net_output);

    motor_command_t motor_data;
    motor_data.left = (u08)(net_output.left * 100);
    motor_data.right = (u08)(net_output.right * 100);

    return motor_data;
}

#endif