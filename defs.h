#ifndef DEFS_H
#define DEFS_H


typedef unsigned char u08;

#define U08_MIN 0
#define U08_MAX 255

// Pin map
#define LIGHT_SENSOR_PIN_RIGHT 0
#define LIGHT_SENSOR_PIN_LEFT 1

#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

#define PHOTO_DIODE_RIGHT 0
#define PHOTO_DIODE_LEFT 1

#define LINE_SENSOR_RIGHT 3
#define LINE_SENSOR_LEFT 2

#define BTN2_PIN DIGITAL9_PIN

#define PHOTO_DIODE_RIGHT_MIN U08_MIN
#define PHOTO_DIODE_RIGHT_MAX U08_MAX
#define PHOTO_DIODE_LEFT_MIN U08_MIN
#define PHOTO_DIODE_LEFT_MAX U08_MAX
#define LINE_SENSOR_RIGHT_MIN U08_MIN
#define LINE_SENSOR_RIGHT_MAX U08_MAX
#define LINE_SENSOR_LEFT_MIN U08_MIN
#define LINE_SENSOR_LEFT_MAX U08_MAX

typedef struct motor_command{
    u08 left;
    u08 right;
}motor_command_t;

typedef struct light_data{
    u08 left;
    u08 right;
}light_data_t;

typedef struct line_sensor_data{
    u08 left;
    u08 right;
}line_sensor_data_t;

float square(float x){
    return x * x;
}

#endif