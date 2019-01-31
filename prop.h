#ifndef PROP_H
#define PROP_H

#include <stdio.h>

// Our libs
#include "delay.h"
#include "hardware.h"


#define THRESHOLD 200
#define BASE_MOTOR_SPEED 25
#define P_TERM 10

struct motor_command compute_proportional(uint8_t left, uint8_t right){
    struct motor_command output;
    output.left = 0;
    output.right = 0;

    

    return output;
}

#endif