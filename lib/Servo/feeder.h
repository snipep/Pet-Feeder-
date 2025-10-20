#ifndef FEEDER_H
#define FEEDER_H
#include <Servo.h> 

#define SERVO_PIN D6 

extern Servo myServo; 


void initServo();
 bool Feed();

#endif
