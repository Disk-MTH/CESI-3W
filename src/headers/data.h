#ifndef WWW_DATA_H
#define WWW_DATA_H

#include "dataTypes.h"
#include "MinimumSerial.h"
#include "SdFat.h"
#include "ChainableLED.h"
#include "DS1307.h"
#include "BME280I2C.h"

extern MinimumSerial minSerial;
extern Button buttons[2];
extern SdFat sd;
extern SdFile logFile;
extern ChainableLED led;
extern DS1307 clock;
extern BME280I2C bme;

extern Mode mode;
extern Config config;
extern LedState ledState;

extern unsigned long lastMillisTick;
extern unsigned long lastMillisLog;
extern unsigned long configAfkCount;
extern bool askForPrompt;

extern LedStateData ledStateData[10];

#endif //WWW_DATA_H
