// Name:  Jared Peter-Contesse & Spencer Shaw
// Description: Hardware Test Program

#include "globals.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// Custum delay lib
#include "delay.h"
#include "hardware.h"

#define PHOTO_DIODE_RIGHT 0
#define PHOTO_DIODE_LEFT 1
#define LINE_SENSOR_RIGHT 3
#define LINE_SENSOR_LEFT 2

void poll_linesensor(u08 pin_num);
void poll_photodiode(u08 pin_num);
void print_data(u08 count, u08 vals);

int main(void)
{
	init();
    motor(0, 0);
    motor(1, 0);

	while(1){
		clear_screen();
		lcd_cursor(0, 0);
		poll_photodiode(PHOTO_DIODE_RIGHT);
		poll_photodiode(PHOTO_DIODE_LEFT);
		lcd_cursor(0, 1);
		poll_linesensor(LINE_SENSOR_RIGHT);
		poll_linesensor(LINE_SENSOR_LEFT);
		delay_ms(500);
	}
}

void poll_photodiode(u08 pin_num){
	print_data(pin_num, analog(pin_num));
}

void poll_linesensor(u08 pin_num){
	print_data(pin_num, analog(pin_num));
}

void print_data(u08 count, u08 vals){
	//print_num(count);
	print_string(":");
	print_num(vals);
}