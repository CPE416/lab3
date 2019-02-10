// Name:  Jared Peter-Contesse & Spencer Shaw
// Lab 2 Part 3
// Description: 


// Custum delay lib
#include "delay.h"
#include "hardware.h"
#include "line_follow_pid.h"
#include "neural.h"

// Settings
#define DELAY_MS 200 // Delay time for loop
#define DELAY_LOOP 10

#define MODE_3A 1 
#define MODE_3B 0

#define DEAD_ZONE 200
#define LEARNING_RATE 0.02

#define MODE_PROP 0
#define MODE_NEURAL 1

void process_data_mode_a(u08* vals, u08* data);
void process_data_mode_b(u08* vals, u08* data);
u08 map_light_to_motor(u08 data);
u08 set_mode(u08 mode);
void print_neural(int count);
void print_prop(int count);


int main(void)
{
    // Init board
    init();

    // Loop variables
    line_data_t line_data;
    motor_command_t motors;
    int data_counter = 0;

    motors.left = 0;
    motors.right = 0;
    halt();

    // u08 mode = set_mode(MODE_3A);
    // u08 vals[] = {0, 0}; // Data retrieve from light sensors
    // u08 data[] = {0, 0}; 
    // Place text at start of buffer
    // sprintf(buffer, "416");

    neural_net_t net;
    init_net(&net, LEARNING_RATE);

    u08 mode = MODE_PROP;

    clear_screen();
    print_string("Part 3");

    //Button Press Before Start
    while((get_btn() == 0) && (get_btn2() == 0)){
        delay_ms(1);
    }

    clear_screen();
    print_string("Prop");

    // Delay before starting state machine
    delay_ms(200);
    // Main loop
    while (1)
    {
        mode = set_mode(mode);

        // Print Mode and training iteration count
        if(mode == MODE_PROP){
            print_prop(data_counter);
        }else{
            print_neural(data_counter);
        }

        // Read line data
        line_data = read_line_sensor();
        // Compute Proportional
        motors = compute_proportional(line_data.left, line_data.right);
        // Train Net
        train_net(line_data, &net, motors);
        // Count of iterations
        data_counter++;
        
        // Control motor based on mode
        switch (mode){
            case MODE_PROP:
                set_motors(motors);
                break;
            case MODE_NEURAL:  
                motors = compute_neural_network(line_data, net);
                set_motors(motors);
                break;
        }

        // Delay
        delay_ms(DELAY_LOOP);
        

        // u08 left = data[0];
        // u08 right = data[1];
        // set_motors(left, right);

        // print_4(vals[0], left, vals[1], right);
        
        // delay_ms(DELAY_MS);

        // mode = set_mode(mode);
    }
    return 1;
}

void process_data_mode_a(u08* vals, u08* data){
    data[0] = map_light_to_motor(vals[0]);
    data[1] = map_light_to_motor(vals[1]);
}

void process_data_mode_b(u08* vals, u08* data){
    data[0] = map_light_to_motor(vals[1]);
    data[1] = map_light_to_motor(vals[0]);
}

u08 map_light_to_motor(u08 val){
    if (val < DEAD_ZONE){
        val = 0;
    }else{
        val = val - DEAD_ZONE;
    } 
    u08 data = (35 - val);
    if (data > 100){
        data = 0;
    }
    return data;
}

u08 set_mode(u08 mode){
    u08 pressed = ((get_btn() == 1) | (get_btn2() == 1));
    if(pressed){
        switch (mode){
            case MODE_PROP:
                mode = MODE_NEURAL;
                break;
            case MODE_NEURAL:
                mode = MODE_PROP;
                break;
            default:
                mode = MODE_PROP;
        }
        delay_ms(500);
    }
    return mode;
    // u08 pressed = get_btn();
    // if(pressed){
    //     if (mode == MODE_3A)
    //         mode = MODE_3B;
    //     else 
    //         mode = MODE_3A;
    // }
    // switch (mode ){
    //     case MODE_3A:
    //         led_off(1);
    //         led_on(0);
    //         break;
    //     case MODE_3B:
    //         led_off(0);
    //         led_on(1);
    //         break;
    //     }
    // return mode;
}

void print_prop(int count){
    clear_screen();
    print_string("Prop");
    lcd_cursor(0, 1);
    print_num(count);
}

void print_neural(int count){
    clear_screen();
    print_string("Neural");
    lcd_cursor(0, 1);
    print_num(count);
}

void print_training3(int count1, int count2){
    clear_screen();
    print_string("Training");
    lcd_cursor(0, 1);
    print_num(count1);
    lcd_cursor(4, 1);
    print_num(count2);
}
