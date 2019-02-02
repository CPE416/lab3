// Description: PID Controller

#ifndef LINE_FOLLOW_PID
#define LINE_FOLLOW_PID

#include <stdio.h>

// Our libs
#include "delay.h"
#include "hardware.h"

#define THRESHOLD 200
#define BASE_MOTOR_SPEED 25
#define P_TERM 1
#define I_TERM 0
#define D_TERM 0
#define ERROR_CHECK_DELAY 50
#define HISTORY_LENGTH 8

typedef struct pid 
{ 
   int *error;
   u08 p_term;
   u08 i_term;
   u08 d_term;
}pid;

// Takes in a PID struct, and a tuple of sensor values 
// sensor[0] = right, sensor[1] = left
// and returns motor values
// motor[0] = right, motor[1] = left
int *pid_control(pid *_pid, int *sensor){
	int actual = 0;
	int derivative;
	int output;
	
	int left_motor = BASE_MOTOR_SPEED;
	int right_motor = BASE_MOTOR_SPEED;
	//u08 right = poll_analog_pin(LINE_SENSOR_RIGHT);
	//u08 left = poll_analog_pin(LINE_SENSOR_LEFT);

	actual += sensor[0];
	actual -= sensor[1];

	insert_error(_pid, actual);

	output = _pid->error[0] * _pid->p_term;
	derivative = calc_derivative(_pid) * _pid->d_term;
	
	left_motor += output;
	right_motor -= output;

	left_motor += derivative;
	right_motor -= derivative;

	//print_4(left, left_motor, right, right_motor);

	//set_motors(left_motor, right_motor);
	sensor[0] = right_motor;
	sensor[1] = left_motor;
	return sensor;
}

//Inserts most recent term and removes last term
void insert_error(pid *_pid, int _error){
	u08 i = HISTORY_LENGTH - 2;
	while(i < HISTORY_LENGTH - 1){
		_pid->error[i] = _pid->error[i+1];
		i--;
	}
	_pid->error[0] = _error;
}

// Prints some values from a PID struct to the LCD
void print_pid(struct pid _pid){
	print_4(_pid.error[0], _pid.p_term, _pid.i_term, _pid.d_term);
}

//Calculates the derivative term based off past error values
int calc_derivative(pid *_pid){
	int derivative = 0;
	u08 i = HISTORY_LENGTH - 1;
	while(i < HISTORY_LENGTH - 1){
		derivative += _pid->error[i];
		i--;
	}
	derivative = derivative / HISTORY_LENGTH;
	return derivative;
}

#endif

