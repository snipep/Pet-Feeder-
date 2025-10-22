#include <Arduino.h>
#include <WiFi.h>

#include "ConfigManager.h"
#include "ServoController.h"
#include "RTCClock.h"
#include "FeederScheduler.h"

#define WIFI_SSID "OnePlusV"
#define WIFI_PASS "1234567890"
#define SERVO_PIN 12
const char* TIMEZONE = "CST6CDT,M3.2.0/2,M11.1.0/2";

// ---- Dependency Instantiation ----
// Create instance if all our care classes
ConfigManager configManager;
ServoController servoController(SERVO_PIN);
RTCClock rtcClock;
FeederSchedular feederScheduler(configManager, rtcClock, servoController);

void connectWiFi() {
  Serial.print("Attempting to connect to WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts > 10) {
      Serial.println("FATAL: WiFi connection failed/timed out!");
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nFATAL: WiFi connected Successfully");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }else {
    Serial.println("\nClocl will be uninitialized. Feeding schedule will not function.");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n--- Cat Feeder System Booting ---");

  // 1. Initialize Configurations (NVS/EEPROM)  
  if (!configManager.begin()) {
    Serial.println("Error: Failed to initialize configuration manager");
  }

  // 2. Initialize Hardware Abstraction
  servoController.initialize();

  // 3. Connect to WiFi
  connectWiFi();

  // 4. Initialize RTC Clock (Time Sync via NTP)
  // This must happen after WiFi is connected
  rtcClock.initialize();
  
    // The FeederScheduler was initialized in its constructor, 
    // now it's ready to run in the loop().

    //Example: Print current loaded config
    FeedingConfig cfg = configManager.getConfig();
    Serial.printf("\nSystem Initialized with: Start: %d, End: %d, Portions: %d, g/s: %.2f\n",
                cfg.dailyStartHour, cfg.dailyEndHour, cfg.numDailyPortions, cfg.gramPerSecond);

    Serial.println("--- System Ready. Checking schedule in loop... ---\n");

}

void loop() {
    // Check if the current time warrants a feed.
    // This method handles all the core business logic (time checks, calculations, servo calls).
    feederScheduler.checkAndFeed();
    
    // Delay to prevent excessive checking (e.g., check every 5 seconds)
    delay(5000); 
}