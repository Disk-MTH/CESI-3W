#include "headers/misc.h"
#include "headers/data.h"

bool initSD() {
    minSerial.print(F("Initializing SD card..."));
    if (!sd.begin(4)) {
        minSerial.println(F("Failed!"));
        return false;
    }
    minSerial.println(F("Done!"));
    return true;
}

void closeSD() {
    sd.end();
    minSerial.println(F("SD card closed!"));
}

void setLedState(LedState state) {
    ledStateData[ledState].colorIndex = 0;
    ledStateData[ledState].millisLeft = 0;
    ledState = state;
}

void readBme280() {
    float temp(NAN), hum(NAN), pres(NAN);
    bme.read(pres, temp, hum, BME280::TempUnit_Celsius, BME280::PresUnit_hPa);

    if (isnan(temp))
        minSerial.println(F("Failed to read temperature!"));
    else if (isnan(hum))
        minSerial.println(F("Failed to read humidity!"));
    else if (isnan(pres))
        minSerial.println(F("Failed to read pressure!"));
    else {
        minSerial.print(F("Temperature: "));
        minSerial.print(temp);
        minSerial.print(F(" °C, Humidity: "));
        minSerial.print(hum);
        minSerial.print(F(" %, Pressure: "));
        minSerial.print(pres);
        minSerial.println(F(" hPa"));
    }
}

void buttonPressed(Button button, unsigned short pressDuration) {
    /*switch (mode) {
        case STANDARD_MODE:
            if (button.pin == buttons[0].pin) {
                minSerial.println(F("Entering maintenance mode..."));
                mode = MAINTAIN_MODE;
                setLedState(LED_MAINTAIN_MODE);
            } else if (button.pin == buttons[1].pin) {
                minSerial.println(F("Entering config mode..."));
                mode = CONFIG_MODE;
                setLedState(LED_CONFIG_MODE);
            }
            break;
        case ECO_MODE:
            break;
        case CONFIG_MODE:
            break;
        case MAINTAIN_MODE:
            minSerial.println(F("Entering standard mode..."));
            mode = STANDARD_MODE;
            setLedState(LED_STANDARD_MODE);
            initSD();
            break;
    }*/

    minSerial.print(F("Button "));
    minSerial.print(button.pin);
    minSerial.print(F(" pressed for "));
    minSerial.print(pressDuration);
}

void logConfig() {
    minSerial.println(F("--- Config ---"));

    minSerial.print(F("Misc - Log interval: "));
    minSerial.print(config.logIntervalMin);
    minSerial.print(F(", Timout: "));
    minSerial.print(config.timeoutSec);
    minSerial.print(F(", Max file size: "));
    minSerial.println(config.fileMaxSizeKo);

    minSerial.print(F("Luminosity sensor - Enable: "));
    minSerial.print(bool(config.lumSensorEnable) ? "true" : "false");
    minSerial.print(F(", Low: "));
    minSerial.print(config.lumSensorLow);
    minSerial.print(F(", High: "));
    minSerial.println(config.lumSensorHigh);

    minSerial.print(F("Temperature sensor - Enable: "));
    minSerial.print(bool(config.tempSensorEnable) ? "true" : "false");
    minSerial.print(F(", Low: "));
    minSerial.print(config.tempSensorLow);
    minSerial.print(F(", High: "));
    minSerial.println(config.tempSensorHigh);

    minSerial.println(F("--- End of config ---"));
}

void logClock(bool date, bool day, bool hour) {
    if (day) {
        switch (clock.dayOfWeek) {
            case MON:
                minSerial.print(F("Monday"));
                break;
            case TUE:
                minSerial.print(F("Tuesday"));
                break;
            case WED:
                minSerial.print(F("Wednesday"));
                break;
            case THU:
                minSerial.print(F("Thursday"));
                break;
            case FRI:
                minSerial.print(F("Friday"));
                break;
            case SAT:
                minSerial.print(F("Saturday"));
                break;
            case SUN:
                minSerial.print(F("Sunday"));
                break;
            case 0:
                minSerial.print(F("N/A"));
                break;
        }
        minSerial.print(F(" "));
    }
    if (date) {
        minSerial.print(clock.dayOfMonth);
        minSerial.print(F("/"));
        minSerial.print(clock.month);
        minSerial.print(F("/"));
        minSerial.print(clock.year + 2000);
        minSerial.print(F(" "));
    } if (hour) {
        minSerial.print(clock.hour);
        minSerial.print(F("h"));
        minSerial.print(clock.minute);
        minSerial.print(F("m"));
        minSerial.print(clock.second);
        minSerial.print(F("s"));
    }
    minSerial.println();
}