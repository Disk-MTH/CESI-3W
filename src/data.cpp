#include "dataTypes.cpp"
#include "ChainableLED.h"

static LedStateData ledStateData[] = {
        { // LED_STANDARD_MODE
                new Color[1]{{0, 255, 0, 255}}, 1
        },
        { // LED_ECO_MODE
                new Color[1]{{00, 0, 255, 255}}, 1
        },
        { // LED_CONFIG_MODE
                new Color[1]{{255, 127, 0, 255}}, 1
        },
        { // LED_MAINTAIN_MODE
                new Color[1]{{255, 50, 0, 255}}, 1
        },
        { // LED_RTC_ERROR
                new Color[2]{
                        {255, 0, 0, 1},
                        {0, 0, 255, 1}
                }, 2
        },
        { // LED_GPS_ERROR
                new Color[2]{
                        {255, 0, 0, 1},
                        {255, 127, 0, 1}
                }, 2
        },
        { // LED_SENSOR_ERROR
                new Color[2]{
                        {255, 0, 0, 1},
                        {0, 255, 0, 1}
                }, 2
        },
        { // LED_SD_ERROR
                new Color[2]{
                        {255, 0, 0, 1},
                        {255, 255, 255, 2}
                }, 2
        },
        { // LED_INVALID_SENSOR_DATA
                new Color[2]{
                        {255, 0, 0, 1},
                        {0, 255, 0, 2}
                }, 2
        },
        { // LED_SD_FULL
                new Color[2]{
                        {255, 0, 0, 1},
                        {255, 255, 255, 1}
                }, 2
        },
};

static ChainableLED led = ChainableLED(2, 3, 1);

static Mode mode = STANDARD_MODE;
static Config config = Config();
static LedState ledState = LED_STANDARD_MODE;

static unsigned long lastLedTick = 0;