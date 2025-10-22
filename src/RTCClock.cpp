#include "RTCClock.h"

// Standard C library includes for time
#include <time.h> 
#include <sys/time.h> // Common source for setenv/tzset on embedded systems

// ESP32 specific time config functions
#include "esp_sntp.h"

// NTP Configuration constants
static const char* ntpServer = "pool.ntp.org";
static const long gmtOffset_sec = 3600;  // 1 hour base offset (can be calculated from TZ string)
static const int daylightOffset_sec = 3600; // 1 hour daylight saving offset

// --- Constructor ---
RTCClock::RTCClock(const char* tzInfo) : timeZone(tzInfo) {}

// ----------------------
// 1. Initialization
// ----------------------
void RTCClock::initialize() {
    Serial.println("Initializing RTC Clock and Time Zone.");
    
    // Set the time zone using standard C library functions (more reliable than configTzFile)
    setenv("TZ", timeZone, 1);
    tzset();
    Serial.printf("Time Zone set to: %s\n", timeZone);

    // Configure the NTP client (requires Wi-Fi connection)
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // Initial check for time sync
    checkNtpStatus();
}

// 2. Check NTP Status
void RTCClock::checkNtpStatus() {
    int attempts = 10;
    while (attempts--) {
        time_t now = time(nullptr);

        if (now > 1000000000) {
            Serial.println("RTC: Time synchronized successfully!");
            timeSynchronized = true;        

            struct tm timeinfo;
            localtime_r(&now, &timeinfo);
            Serial.printf("Local Time: %s", asctime(&timeinfo));
            return;
        }
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nRTC: Time synchronization failed/timed out. Using internal clock (may be wrong).");
    timeSynchronized = false;
}

// 3. Get Current Time
time_t RTCClock::getCurrentTime() {
    if (!timeSynchronized) {
        checkNtpStatus();
    }
    time_t now;
    time(&now);
    return now;
}

// 4. Time Set Check
bool RTCClock::isTimeSet() {
    return timeSynchronized && (getCurrentTime() > 1000000000);
}