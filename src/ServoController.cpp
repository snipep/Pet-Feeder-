#include "ServoController.h"

// Constructor
ServoController::ServoController(int pin) : servoPin(pin){
    // Member intitializer list 
    // for intitializing class members.
}

void ServoController::initialize() {
    Serial.print("Initializing servo on pin: ");
    Serial.println(servoPin);

    servoMotor.attach(servoPin);
        
    returnHome();
}

void ServoController::dispense(long durationMs) {
    Serial.print("Dispensing started. Duration: " );
    Serial.println(durationMs);
    Serial.println(" ms.");

    // 1. Move to Dispense Position (90 degrees)
    moveServo(DISPENSE_ANGLE);

    // 2. Hold Position for Dispense Duration
    // The duration is calculated based on the desired grams and the g/s constant.
    // We only dispense food during this delay.
    delay(durationMs - (MOVEMENT_DELAY_MS * 2));

    // 3. Move back to Home Position
    returnHome();

    Serial.println("Dispense complete.");
}

// Internal Helper: Smooth Servo Movement
void ServoController::moveServo(int angle) {
    if (servoMotor.attached()) {
        servoMotor.write(angle); 

        // Wait briefly for the physical movement to complete.
        // This is crucial to prevent the main loop from running logic 
        // while the servo is in motion.
        delay(MOVEMENT_DELAY_MS);
    }
}

void ServoController::returnHome() {
    Serial.print("Returning servo to home angle (");
    Serial.print(HOME_ANGLE);
    Serial.println(" degrees)");
    moveServo(HOME_ANGLE);
}