#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>

struct FeedingConfig {
    public:
        // Time range for feeding (in hours)
        int dailyStartHour;
        int dailyEndHour;

        // Portion Configuration
        int numDailyPortions;
        
        // Represents the amount of food(gm) despense in sec
        float gramPerSecond;
};

class ConfigManager {
    public:
        ConfigManager();
        bool begin();
        FeedingConfig getConfig();
        bool saveConfig(const FeedingConfig& newConfig);
    private:
        const char* CONFIG_KEY = "FeedingData";
        const char* PREF_NAMESPACE = "CatFeederCfg";
        FeedingConfig currentConfig;
        bool loadFromNVS();
};

#endif