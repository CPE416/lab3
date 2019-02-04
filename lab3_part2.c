// Name:  Jared Peter-Contesse & Spencer Shaw
// Lab 3 Part 2
// Description: 

#include <stdio.h>

#include "delay.h"
#include "hardware.h"
#include "line_follow_pid.h"
#include "prop.h"
#include "neural.h"

// Settings
#define DELAY_MS 100 // Delay time for loop

#define MODE_PROP 1 
#define MODE_NEURAL 0

u08 set_mode(u08 mode);

int main(void)
{
    init();
    line_sensor_data_t line_data;
    motor_command_t motors;
    motors.left = 0;
    motors.right = 0;
    set_motors(motors);

    neural_net_t net;
    init_net(&net);

    while (1){
    	line_data = read_line_sensor();
        motors = compute_neural_network(line_data, net);
        set_motors(motors);

        print_4(line_data.left, motors.left, line_data.right, motors.right);
        delay_ms(DELAY_MS);
    }
    return 27;
}


u08 set_mode(u08 mode){
    u08 pressed = get_btn();
    if(pressed){
        if (mode == MODE_PROP)
            mode = MODE_NEURAL;
        else 
            mode = MODE_PROP;
    }
    switch (mode){
        case MODE_PROP:
            led_off(1);
            led_on(0);
            break;
        case MODE_NEURAL:
            led_off(0);
            led_on(1);
            break;
        }
    return mode;
}
