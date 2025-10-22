#include "FeederScheduler.h"
#include <time.h>


// ---- Contstructor ----
// FeederSchedular::FeederSchedular(ConfigManager &cfgMgr, RTCClock &clock, ServoController &servo) : configManager(cfgMgr), clock(clock), servoController(servo)
FeederSchedular::FeederSchedular(ConfigManager& cfgMgr, RTCClock& clock, ServoController& servo) : configManager(cfgMgr), clock(clock), servoController(servo)
{
    calculateScheduleMetrics();
}

// 1. Calculate Schedule Metrics
void FeederSchedular::calculateScheduleMetrics()
{
    FeedingConfig cfg = configManager.getConfig();
    
    if (cfg.numDailyPortions <= 0)
    {
        Serial.println("Warning: Num daily portions is zerp or less. Setting to 1.");
        cfg.numDailyPortions = 1;
    }

    // 1. Calculate Portion Size(gm)
    desiredPortionG = TOTAL_DAILY_FOOD_G / cfg.numDailyPortions;

    // 2. Calculate Dispense duration(ms)
    // Formula: Duration = (Grams / Grams per Second) * 1000
    if (cfg.gramPerSecond > 0)
    {
        despenseDurationMs = (long)((desiredPortionG / cfg.gramPerSecond) * 1000);
    }
    else
    {
        Serial.println("FATAL: Grams per second is zero. Cannot dispense!");
        despenseDurationMs = 0;
    }

    // 3. Calculate Time Between Feeds(s)
    int totalFeedingHours = cfg.dailyEndHour - cfg.dailyStartHour;
    if (totalFeedingHours < 1)
        totalFeedingHours += 24;

    long totalFeedingSeconds = totalFeedingHours * 3600;

    // Dividing feeding window by the number of intervals (Portions - 1 + 1 (for initital delay))
    timeBetweenFeedsSec = totalFeedingSeconds / cfg.numDailyPortions;

    Serial.println("--- Scheduler Metrics Calculated ---");
    Serial.printf("  Portion Size: %.2f g\n", desiredPortionG);
    Serial.printf("  Dispense Duration: %ld ms\n", despenseDurationMs);
    Serial.printf("  Interval: %ld seconds\n", timeBetweenFeedsSec);
    Serial.println("------------------------------------");
}

// 2. Window Check
bool FeederSchedular::isWithinFeedingWindow(time_t now_s)
{
    FeedingConfig cfg = configManager.getConfig();

    struct tm *timeinfo = localtime(&now_s);

    if (timeinfo == NULL) return false;

    int currentHour = timeinfo->tm_hour;

    // The logic assume feedin only happen if the current hour is >= Start AND < END
    if (cfg.dailyStartHour < cfg.dailyEndHour) {
        return (currentHour >= cfg.dailyStartHour && currentHour < cfg.dailyEndHour);
    } else {
        return (currentHour >= cfg.dailyStartHour || currentHour < cfg.dailyEndHour);
    }
}

// 3. Execute Feed
void FeederSchedular::executeFeed()
{
    Serial.println("---- Feeding Time ----");

    //1. Perform the dispense action
    servoController.dispense(despenseDurationMs);

    //2. Update the last feed time to the current time
    lastFeedTime = clock.getCurrentTime();

    Serial.println("Feed cycle completed. Last feed time updated");
}

// 4. Main Check Routine
void FeederSchedular::checkAndFeed()
{
    // time_t now = rtcClock.getCurrentTime();
    time_t now = 0;
    if (now == 0)
    {
        return;
    }

    if (now - lastFeedTime > timeBetweenFeedsSec)
    {
        if (isWithinFeedingWindow(now))
        {
            executeFeed();
        }
        else
        {
            if (lastFeedTime != 0)
            {
                Serial.println("Interval passed, but outside feeding window. Skipping feed.");
            }
            lastFeedTime = now;
        }
    }
}
