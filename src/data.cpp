#include "limits.h"
#include "dataTypes.cpp"
#include "ChainableLED.h"
#include "DS1307.h"
#include "BME280I2C.h"

static LedStateData ledStateData[] = {
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

static Button buttons[] = {
        {4, 1000},
        {5, 3000},
};
static ChainableLED led = ChainableLED(2, 3, 1);
static DS1307 clock;
static BME280I2C bme;

static Mode mode = STANDARD_MODE;
static Config config = Config();
static LedState ledState = LED_STANDARD_MODE;

static unsigned long lastMillisTick = 0;
static bool askForPrompt = true;

static unsigned long lastMillisLog = 0;
static unsigned long configAfkCount = 0;