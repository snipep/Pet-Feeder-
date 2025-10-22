#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Arduino.h> 
#include <ESP32Servo.h>


class ServoController {
    public:
        static constexpr int HOME_ANGLE = 0;
        static constexpr int DISPENSE_ANGLE = 90;
        static constexpr long MOVEMENT_DELAY_MS = 300;

    //  @brief Constructor: Initializes the controller with the physical pin.
        ServoController(int pin);
        void initialize();
        void dispense(long durationMs);
        void returnHome();
    
    private:
        int servoPin;
        Servo servoMotor;
        void moveServo(int angle);
};


#endif