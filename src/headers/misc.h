#ifndef WWW_MISC_H
#define WWW_MISC_H

#include "dataTypes.h"

bool initSD();
void closeSD();
void setLedState(LedState state);
void readBme280();
void buttonPressed(Button button);
void logConfig();
void logClock(bool date, bool day, bool hour);

#endif //WWW_MISC_H
