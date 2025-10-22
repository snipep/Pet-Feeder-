#ifndef FEEDER_SCHEDULER_H
#define FEEDER_SCHEDULER_H

#include <Arduino.h>
#include <time.h>
#include "ConfigManager.h"
#include "ServoController.h"
#include "RTCClock.h"

// Forward declarations to endure dependencies are not fully inplemented here
// This is good peractice to minimize compulation time.
class RTCClock;
class ServoController;

class FeederSchedular
{
public:
    // --- Constants for Calculation ---
    static constexpr float TOTAL_DAILY_FOOD_G = 60.0f;

    // @brief Constructor: Initializes the scheduler with its dependencies.
    // @param cfgMgr Reference to the Configuration Manager instance.
    // @param clock Reference to the RTC Clock instance.
    // @param servo Reference to the Servo Controller instance.
    FeederSchedular(ConfigManager &cfgMgr, RTCClock &clock, ServoController &servo);

    //@brief The main routine called in the Arduino loop().
    // Checks the time and triggers a feed if the interval has passed.
    void checkAndFeed();

private:
    //---- Dependencies ----
    ConfigManager &configManager;
    RTCClock &clock;
    ServoController &servoController;

    //---- State Variable ----
    time_t lastFeedTime = 0;

    // Calculated values
    float desiredPortionG;
    long timeBetweenFeedsSec;
    long despenseDurationMs;

    void calculateScheduleMetrics();
    bool isWithinFeedingWindow(time_t now_s);
    void executeFeed();
};

#endif