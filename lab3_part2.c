// Name:  Jared Peter-Contesse & Spencer Shaw
// Lab 3 Part 2
// Description: 

#include <stdio.h>

#include "delay.h"
#include "hardware.h"
#include "line_follow_pid.h"
//#include "prop.h"
#include "neural.h"

// Settings
#define DELAY_MS 100 // Delay time for loop

#define MODE_PROP 0
#define MODE_DATA 1
#define MODE_TRAINING 2 
#define MODE_NEURAL 3

u08 set_mode(u08 mode);

int main(void)
{
    init();

    // Variables
    line_sensor_data_t line_data;
    motor_command_t motors;
    int data_counter = 0;
    motors.left = 0;
    motors.right = 0;
    set_motors(motors);

    neural_net_t net;
    init_net(&net);

    u08 mode = MODE_PROP;

    clear_screen();
    print_string("Part 2");

    //Button Press Before Start
    while((get_btn() == 0) && (get_btn2() == 0)){
        delay_ms(1);
    }

    clear_screen();
    print_string("Proport");
    lcd_cursor(0, 1);
    print_string("ional");

    //Button Press Delay before start
    delay_ms(200);

    while (1){

        // Check for Button Press and change 
        // Modes and Display
        mode = set_mode(mode);

        // Main Loop
        switch(mode){
            case MODE_PROP:

                break;
            case MODE_DATA:

                break;
            case MODE_TRAINING:

                break;
            case MODE_NEURAL:

                break;
            default:
                mode = MODE_PROP;
        }

    	// line_data = read_line_sensor();
     //    motors = compute_neural_network(line_data, net);
     //    set_motors(motors);

     //    print_4(line_data.left, motors.left, line_data.right, motors.right);
     //    delay_ms(DELAY_MS);
    }
    return 27;
}


u08 set_mode(u08 mode){
    u08 pressed = ((get_btn() == 1) | (get_btn2() == 1));
    if(pressed){
        switch (mode){
        case MODE_PROP:
            mode = MODE_DATA;
            halt();
            clear_screen();
            print_string("Data");
            print_num(0);
            break;
        case MODE_DATA:
            mode = MODE_TRAINING;
            halt();
            clear_screen();
            print_string("Training");
            break;
        case MODE_TRAINING:
            mode = MODE_NEURAL;
            halt();
            clear_screen();
            print_string("Neural");
            break;
        case MODE_NEURAL:
            mode = MODE_TRAINING;
            halt();
            clear_screen();
            print_string("Training");
            break;
        default:
            mode = MODE_PROP;
        }
        delay_ms(200);
    }
    return mode;
}
