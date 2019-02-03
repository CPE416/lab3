#ifndef NEURAL_H
#define NEURAL_H

#define INPUT_NODES 2
#define HIDDEN_NODES 3
#define OUTPUT_NODES 2

typedef struct input_node{
    float bias;
}input_node;

typedef struct hidden_node{
    float weights[INPUT_NODES];
    float bias;
}hidden_node;

typedef struct output_node{
    float weights[HIDDEN_NODES];
    float bias;
}output_node;

typedef struct neural_net{
    input_node input_layer[INPUT_NODES];
    hidden_node hidden_layer[HIDDEN_NODES];
    output_node output_layer[OUTPUT_NODES];
}neural_net;

typedef struct input_data{
    u08 left;
    u08 right;
}input_data;

typedef struct net_output{
    float left;
    float right;
}net_output;

void infer_input_layer(float *input_layer_output, input_data input, float *input_layer_bias){

    input_layer_output[0] = (((float) input.left) / 255) - input_layer_bias[0];
    input_layer_output[1] = (((float) input.right) / 255) - input_layer_bias[1];

    // TODO: Finish calculations on this node
}

void infer_hidden_layer(float *hidden_layer_output, float *input_layer_output, float ** hidden_layer_weights, float* hidden_layer_bias){
    u08 hidden_node_index = 0;
    while(hidden_node_index < HIDDEN_NODES){
        hidden_layer_output[hidden_node_index] = 0 - hidden_layer_bias[hidden_node_index];
        u08 input_node_index = 0;
        while(input_node_index < INPUT_NODES){
            hidden_layer_output[hidden_node_index] += (input_layer_output[input_node_index] * hidden_layer_weights[hidden_node_index][input_node_index]);
        }
    }
}

void infer_output_layer(float *output_layer_output, float *hidden_layer_output, float **output_layer_weights, float *output_layer_bias){
    u08 output_node_index = 0;
    while(output_node_index < OUTPUT_NODES){
        output_layer_output[output_node_index] = 0 - output_layer_bias[output_node_index];
        u08 hidden_node_index = 0;
        while(hidden_node_index < HIDDEN_NODES){
            output_layer_output[output_node_index] += (hidden_layer_output[hidden_node_index] * output_layer_weights[output_node_index][hidden_node_index]);
        }
    }
}

neural_net train_net(neural_net net, line_sensor_data line_data, motor_command motor_data){
    
    // TODO: Implement training
    
    return net;
}

net_output infer_net(neural_net net, line_sensor_data line_data){

    // TODO: Implement inference

    net_output output;
    // TODO: Read output of net
    output.left = 0;
    output.right = 1;
    return output;
}

motor_command compute_neural(neural_net net, line_sensor_data line_data){
    net_output net_out_data = infer_net(net, line_data);

    motor_command motor_data;
    motor_data.left = (u08)(net_out_data.left * 100);
    motor_data.right = (u08)(net_out_data.right * 100);

    return motor_data;
}

#endif