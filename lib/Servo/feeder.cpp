#include "feeder.h"
#include <Arduino.h>
#include <Servo.h>

Servo myServo;

void initServo() {
    myServo.attach(SERVO_PIN, 500, 2500); 
    Serial.print("Servo initialized on pin D6");
    // Serial.println(digitalPinToInterrupt(SERVO_PIN));
}

bool Feed() {
    Serial.println("Starting feed cycle (180 degrees)...");
    
    myServo.write(180);
    delay(1000); 
    
    myServo.write(0);
    delay(1000); 

    Serial.println("Feed cycle complete.");
    
    return true; 
}
