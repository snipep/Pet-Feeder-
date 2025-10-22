#include "ConfigManager.h"
#include <Preferences.h>

// Global object for NVS access
Preferences preferences;

ConfigManager::ConfigManager() {
    currentConfig = FeedingConfig{
        .dailyStartHour = 8,
        .dailyEndHour = 20,
        .numDailyPortions = 3,
        .gramPerSecond = 60.00
    };
}

bool ConfigManager::begin() {
    if (!preferences.begin(PREF_NAMESPACE, false)) {
        Serial.println("Error opeing Preferences namespace.");
        return false;
    }

    // Load existing config or use defaults
    loadFromNVS();
    return true;
}

bool ConfigManager::loadFromNVS() {
    if (preferences.isKey(CONFIG_KEY)) {
        // Read the entire struct blick from NVS
        size_t size = preferences.getBytes(CONFIG_KEY, &currentConfig, sizeof(currentConfig));

        if (size == sizeof(currentConfig)) {
            Serial.println("Config loaded successfully from NC");
            Serial.printf("Start Hour: %d, End Hour: %d, Portions: %d, g/s: %.2f\n",
                        currentConfig.dailyStartHour,
                        currentConfig.dailyEndHour, 
                        currentConfig.numDailyPortions,
                        currentConfig.gramPerSecond);
            return true;
        } else {
            Serial.println("NVS data size mismatch/ Using default config");
        }
    } else {
        Serial.println("No configuration found in NVS. Using default config and savijng it.");
        saveConfig(currentConfig);
    }
    return false;
}

FeedingConfig ConfigManager::getConfig() {
    return currentConfig;   
}

bool ConfigManager::saveConfig(const FeedingConfig& newConfig) {
    currentConfig = newConfig;
    size_t writtenBytes = preferences.putBytes(CONFIG_KEY, &currentConfig, sizeof(currentConfig));

    if (writtenBytes == sizeof(currentConfig)) {
        Serial.println("Config saved successfully to NVS");
        return true;
    } else {
        Serial.println("Error saving config to NVS");
        return false;
    }
}