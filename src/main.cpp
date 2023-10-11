#include "Arduino.h"
#include "../lib/ChainableLED/ChainableLED.h"

enum LedState {
    STANDARD_MODE,
    CONFIG_MODE,
    ECO_MODE,
    REPAIR_MODE,
    RTC_ERROR,
    GPS_ERROR,
    SENSOR_ERROR,
    SD_ERROR,
    INVALID_SENSOR_DATA,
    SD_FULL,
};

struct Color {
    byte red;
    byte green;
    byte blue;
    byte durationSec;
};

struct LedStateData {
    Color* colors;
    byte colorCount;
    byte colorIndex;
    long timeLeft;
};

LedStateData ledStateData[] = {
        { // STANDARD_MODE
                new Color[1]{{0, 255, 0, 255}}, 1
        },
        { // CONFIG_MODE
                new Color[1]{{255, 127, 0, 255}}, 1
        },
        { // ECO_MODE
                new Color[1]{{00, 0, 255, 255}}, 1
        },
        { // REPAIR_MODE
                new Color[1]{{255, 50, 0, 255}}, 1
        },
        { // RTC_ERROR
                new Color[2]{
                        {255, 0, 0, 1},
                        {0, 0, 255, 1}
                }, 2
        },
        { // GPS_ERROR
                new Color[2]{
                        {255, 0, 0, 1},
                        {255, 127, 0, 1}
                }, 2
        },
        { // SENSOR_ERROR
                new Color[2]{
                        {255, 0, 0, 1},
                        {0, 255, 0, 1}
                }, 2
        },
        { // SD_ERROR
                new Color[2]{
                        {255, 0, 0, 1},
                        {255, 255, 255, 2}
                }, 2
        },
        { // INVALID_SENSOR_DATA
                new Color[2]{
                        {255, 0, 0, 1},
                        {0, 255, 0, 2}
                }, 2
        },
        { // SD_FULL
                new Color[2]{
                        {255, 0, 0, 1},
                        {255, 255, 255, 1}
                }, 2
        },
};

ChainableLED led = ChainableLED(2, 3, 1);
LedState currentState = SD_ERROR;
unsigned long lastLedTick = 0;

void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("Initialization");

    led.init();
}

void loop() {
    if (millis() - lastLedTick > 1000) {
        if (ledStateData[currentState].timeLeft == 0) {
            led.setColorRGB(0, ledStateData[currentState].colors[ledStateData[currentState].colorIndex].red,
                            ledStateData[currentState].colors[ledStateData[currentState].colorIndex].green,
                            ledStateData[currentState].colors[ledStateData[currentState].colorIndex].blue);

            ledStateData[currentState].timeLeft = ledStateData[currentState].colors[ledStateData[currentState].colorIndex].durationSec;

            if (ledStateData[currentState].colorIndex == ledStateData[currentState].colorCount - 1)
                ledStateData[currentState].colorIndex = 0;
            else
                ledStateData[currentState].colorIndex++;
        }
        ledStateData[currentState].timeLeft--;
        lastLedTick = millis();
    }
}
