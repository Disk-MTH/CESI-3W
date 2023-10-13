#include "Arduino.h"

enum Mode {
    STANDARD_MODE,
    ECO_MODE,
    CONFIG_MODE,
    MAINTAIN_MODE,
};

enum LedState {
    LED_STANDARD_MODE,
    LED_ECO_MODE,
    LED_CONFIG_MODE,
    LED_MAINTAIN_MODE,
    LED_RTC_ERROR,
    LED_GPS_ERROR,
    LED_SENSOR_ERROR,
    LED_SD_ERROR,
    LED_INVALID_SENSOR_DATA,
    LED_SD_FULL,
};

struct Config {
    byte logIntervalMin = 10;
    byte timeoutSec = 10;
    int fileMaxSizeO = 2;
    bool lumSensorEnable = true;
    int lumSensorLow = 255;
    int lumSensorHigh = 768;
    bool tempSensorEnable = true;
    int tempSensorLow = -10;
    int tempSensorHigh = 60;
    bool humSensorEnable = true;
    int humSensorLow = 0;
    int humSensorHigh = 50;
};

struct Button {
    byte pin;
    unsigned short durationMillis;
    unsigned short millisLeft;
    bool isPressed;
};

struct Color {
    byte red;
    byte green;
    byte blue;
    unsigned short durationMillis;
};

struct LedStateData {
    Color* colors;
    byte colorCount;
    byte colorIndex;
    unsigned short millisLeft;
};