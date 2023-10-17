#ifndef WWW_MISC_H
#define WWW_MISC_H

#include "dataTypes.h"
#include "MinimumSerial.h"

void initSD();
void closeSD();
void setLedState(LedState state);
String getRTC(bool day, bool date, bool hour);
String getBME();
void buttonPressed(Button button, unsigned short pressDuration);
void logConfig();

#endif //WWW_MISC_H
