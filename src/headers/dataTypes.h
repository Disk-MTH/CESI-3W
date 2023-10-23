#ifndef WWW_DATATYPES_H
#define WWW_DATATYPES_H

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
    LED_SENSOR_ERROR,
    LED_SD_ERROR,
    LED_INVALID_SENSOR_DATA,
    LED_SD_FULL,
};

struct Config {
    byte logIntervalMin = 10;
    byte fileMaxSizeKo = 2;
    bool lumSensorEnable = true;
    int lumSensorLow = 255;
    int lumSensorHigh = 768;
    bool tempSensorEnable = true;
    int tempSensorLow = -10;
    int tempSensorHigh = 60;
    bool humSensorEnable = true;
    int humSensorLow = 0;
    int humSensorHigh = 50;
    bool presSensorEnable = true;
    int pressSensorLow = 850;
    int pressSensorHigh = 1080;
};

struct Button {
    byte pin;
    unsigned short pressDuration;
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

#endif //WWW_DATATYPES_H
