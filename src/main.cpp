#include <Arduino.h>
#include "feeder.h"

void setup() {
  Serial.begin(115200);
  Serial.println("\n--- ESP8266 Feeder Started ---");
  initServo();
}

void loop() {
  // Check if feeding was successful (it will always be true now)
  if (!Feed()) {
    Serial.println("Feeding failed (Should not happen with current logic)");
  }
  
  // Wait for a few seconds before trying to feed again
  delay(10000); 
}
