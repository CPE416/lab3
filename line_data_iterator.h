#ifndef LINE_DATA_ITER_H
#define LINE_DATA_ITER_H

#include "defs.h"

#define MIN_LINE_VAL (0)
#define MAX_LINE_VAL (255)
#define INCREMENT (5)

int left_val;
int right_val;

void init_line_data_iter(){
    left_val= 0;
    right_val = 0;
}

line_data_t get_line_iter(){
    line_data_t line_data;
    line_data.left = left_val;
    line_data.right = right_val;
    return line_data;
}

u08 continue_epoch(){
    return left_val < MAX_LINE_VAL;
}

void increment_iter(){
    right_val += INCREMENT;
    if (right_val > MAX_LINE_VAL){
        right_val = MIN_LINE_VAL;
        left_val += INCREMENT;
    }
    if(left_val > MAX_LINE_VAL){
        left_val = MIN_LINE_VAL;
    }
}

#endif