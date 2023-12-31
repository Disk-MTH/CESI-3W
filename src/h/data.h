#ifndef WWW_DATA_H
#define WWW_DATA_H

#include "limits.h"
#include "dataTypes.h"
#include "SoftwareSerial.h"
#include "SdFat.h"
#include "ChainableLED.h"
#include "DS1307.h"
#include "BME280I2C.h"

#define DONE F("Done!")
#define FAILED F("Failed!")
#define NA F("N/A")
#define SEMI F(";")
#define EEPROM_FLAG 134
#define VERSION F("1.0")
#define LOT_ID F("64d2039e87005ca76307")

extern SoftwareSerial gpsSerial;
extern Button buttons[2];
extern SdFat sd;
extern SdFile logFile;
extern ChainableLED led;
extern DS1307 clock;
extern BME280I2C bme;

extern Mode mode;
extern Config config;
extern LedState ledState;
extern LedStateData ledStateData[10];

extern bool isSdInit;
extern String gps;
extern unsigned long lastMillisTick;
extern unsigned long lastMillisLog;
extern unsigned long configAfkCount;
extern bool askForPrompt;
extern bool shouldLogGps;

#endif //WWW_DATA_H
