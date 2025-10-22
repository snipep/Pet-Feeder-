#ifndef RTC_CLOCK_H
#define RTC_CLOCK_H

#include <Arduino.h>
#include <time.h>

class RTCClock
{
public:
    // @brief Constructor. Sets the default time zone configuration.
    // @param tzInfo The POSIX time zone string (e.g., "PST8PDT,M3.2.0,M11.1.0").
    RTCClock(const char* tzInfo = "CST6CDT,M3.2.0/2,M11.1.0/2");

    // @brief Initializes the clock system, setting up the time zone and NTP access.
    void initialize();
    
    // @brief Retrieves the current time in seconds since the epoch.
    // @return time_t The current time, or 0 if the time has not been set/synced.
    time_t getCurrentTime();
    
    // @brief Checks if the clock has successfully synchronized its time (e.g., via NTP).
    // @return true if time is considered valid (greater than a typical Unix epoch start value).
    bool isTimeSet();
    
private:
    const char* timeZone;
    bool timeSynchronized = false;

    // Internal helper for synchronization status
    void checkNtpStatus();
};

#endif