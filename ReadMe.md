## 🐾 ESP32-Powered Smart Pet Feeder

An automated, time-zone-aware pet food dispenser built on the **ESP32** development platform using the **PlatformIO** ecosystem.

The system manages feeding schedules, calculates portion sizes automatically, and uses Non-Volatile Storage (NVS) to retain configuration settings even after power loss.

### ✨ Features

* **Time Synchronization (NTP):** Uses Wi-Fi to synchronize time via NTP, ensuring the feeding schedule is accurate and time-zone correct.
* **Non-Volatile Configuration:** Feeding parameters are persisted using ESP32's NVS (Non-Volatile Storage) via the `Preferences` library.
* **Scheduled Dispensing:** Automatically calculates feed intervals and dispenses food only within a configured daily time window.
* **Automated Metrics:** Calculates portion size, dispense duration, and time between feeds based on target daily food amount and number of portions.
* **Robust Servo Control:** Uses the `ESP32Servo` library for precise, delayed movement to ensure accurate dispensing and return to the home position.

***

### ⚙️ Hardware Requirements

| Component | Description |
| :--- | :--- |
| **Microcontroller** | ESP32 Development Board (`esp32dev`) |
| **Actuator** | Standard Servo Motor (Connected to **GPIO 12**) |
| **Connectivity** | 2.4 GHz Wi-Fi Access Point |
| **Power Supply** | Suitable power source for ESP32 and Servo Motor |

### 🛠️ Software Setup

This project uses the **PlatformIO** extension for VS Code.

1.  **Install PlatformIO:** Ensure you have PlatformIO installed in your development environment.
2.  **Clone Repository:** Download or clone the project files.
3.  **Install Dependencies:** The necessary libraries are specified in `platformio.ini` and will be installed automatically by PlatformIO:
    * `ESP32Servo @ 1.1.2`
    * `WiFi @ ^2.0.0`

#### Configuration

Before uploading, you must update the following settings in `src/main.cpp`:

| Constant | File | Description | Default Value |
| :--- | :--- | :--- | :--- |
| `WIFI_SSID` | `src/main.cpp` | Your Wi-Fi network name. | `"OnePlusV"` |
| `WIFI_PASS` | `src/main.cpp` | Your Wi-Fi password. | `"1234567890"` |
| `TIMEZONE` | `src/main.cpp` | POSIX timezone string for NTP sync. | `"CST6CDT,M3.2.0/2,M11.1.0/2"` |

### 📊 Core Feeding Configuration

The default feeding schedule parameters are defined in `src/ConfigManager.cpp` and are stored in NVS. These are the key settings that control the feeding logic:

| Parameter | Type | Description | Default Value |
| :--- | :--- | :--- | :--- |
| `dailyStartHour` | `int` | The hour (24-hour format) the feeding window opens. | `8` (8:00 AM) |
| `dailyEndHour` | `int` | The hour (24-hour format) the feeding window closes. | `20` (8:00 PM) |
| `numDailyPortions`| `int` | The number of times the feeder should dispense food daily. | `3` |
| `gramPerSecond` | `float` | Calibration value for the dispense mechanism (grams per second). | `60.00` |

Additionally, the total daily food is set in `include/FeederScheduler.h`:
* `TOTAL_DAILY_FOOD_G`: `60.0f` grams.

### 💡 Scheduling Logic

The `FeederScheduler` calculates the precise timing for each feed based on the configuration:

1.  **Portion Size:** `desiredPortionG` is calculated as `TOTAL_DAILY_FOOD_G` / `numDailyPortions`.
2.  **Dispense Duration:** `despenseDurationMs` is calculated as (`desiredPortionG` / `gramPerSecond`) \* 1000.
3.  **Interval:** `timeBetweenFeedsSec` is calculated by dividing the total feeding window duration (in seconds) by `numDailyPortions`.
4.  The system waits for the calculated interval to pass, and then checks if the current time falls within the configured `dailyStartHour` and `dailyEndHour` window before triggering a feed.