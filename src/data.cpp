#include "limits.h"
#include "dataTypes.cpp"
#include "ChainableLED.h"

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

static Mode mode = STANDARD_MODE;
static Config config = Config();
static LedState ledState = LED_INVALID_SENSOR_DATA;

static unsigned long lastMillisTick = 0;