#ifndef WWW_MISC_H
#define WWW_MISC_H

#include "dataTypes.h"
#include "MinimumSerial.h"

void initSD();
void closeSD();
void setLedState(LedState state);
void readBme280();
void buttonPressed(Button button, unsigned short pressDuration);
void logConfig();
String getFormattedDate(bool day, bool date, bool hour);

#endif //WWW_MISC_H
