#ifndef WWW_MISC_H
#define WWW_MISC_H

#include "dataTypes.h"
#include "MinimumSerial.h"

void initSD();
void closeSD();
void setLedState(LedState state);
void logData(Print& outputStream);
String getRTC(bool date, bool hour);
String getBME(bool temp, bool hum, bool pres);
void buttonPressed(Button button, unsigned short pressDuration);
void logConfig();

#endif //WWW_MISC_H
