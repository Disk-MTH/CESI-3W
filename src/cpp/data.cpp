#include "headers/data.h"

MinimumSerial serial;
SoftwareSerial gpsSerial(5, 6);
Button buttons[] = {
        {4},
        {5},
};
SdFat sd;
SdFile logFile;
ChainableLED led = ChainableLED(2, 3, 1);
DS1307 clock;
BME280I2C bme;

Mode mode = STANDARD_MODE;
Config config = Config();
LedState ledState = LED_STANDARD_MODE;
LedStateData ledStateData[] = {
        { // LED_STANDARD_MODE
                new Color[1]{{0, 255, 0, USHRT_MAX}}, 1
        },
        { // LED_ECO_MODE
                new Color[1]{{00, 0, 255, USHRT_MAX}}, 1
        },
        { // LED_CONFIG_MODE
                new Color[1]{{255, 127, 0, USHRT_MAX}}, 1
        },
        { // LED_MAINTAIN_MODE
                new Color[1]{{255, 50, 0, USHRT_MAX}}, 1
        },
        { // LED_RTC_ERROR
                new Color[2]{
                        {255, 0, 0, 500},
                        {0, 0, 255, 500}
                }, 2
        },
        { // LED_GPS_ERROR
                new Color[2]{
                        {255, 0, 0, 500},
                        {255, 127, 0, 500}
                }, 2
        },
        { // LED_SENSOR_ERROR
                new Color[2]{
                        {255, 0, 0, 500},
                        {0, 255, 0, 500}
                }, 2
        },
        { // LED_SD_ERROR
                new Color[2]{
                        {255, 0, 0, 333},
                        {255, 255, 255, 666}
                }, 2
        },
        { // LED_INVALID_SENSOR_DATA
                new Color[2]{
                        {255, 0, 0, 333},
                        {0, 255, 0, 666}
                }, 2
        },
        { // LED_SD_FULL
                new Color[2]{
                        {255, 0, 0, 500},
                        {255, 255, 255, 500}
                }, 2
        },
};

bool isSdInit = false;
bool isBmeInit = false;
String gps = "";
unsigned long lastMillisTick = 0;
unsigned long lastMillisLog = 0;
unsigned long configAfkCount = 0;
bool askForPrompt = true;
bool missingDataForLog = false;