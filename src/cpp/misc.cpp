#include "headers/misc.h"
#include "headers/data.h"

void initSD() {
    if (isSdInit) return;

    while (!isSdInit) {
        serial.print(F("SD card..."));
        if (!sd.begin(4)) {
            isSdInit = false;
            serial.println(FAILED);
            delay(5000);
        } else {
            isSdInit = true;
            serial.println(DONE);
        }
    }
}

void closeSD() {
    if (!isSdInit) return;

    sd.end();
    isSdInit = false;
    serial.println(F("SD card closed!"));
}

void setLedState(LedState state) {
    ledStateData[ledState].colorIndex = 0;
    ledStateData[ledState].millisLeft = 0;
    ledState = state;
}

String getRTC(bool day, bool date, bool hour) {
    clock.getTime();
    String result = "";
    if (day) {
        switch (clock.dayOfWeek) {
            case MON:
                result += F("Monday");
                break;
            case TUE:
                result += F("Tuesday");
                break;
            case WED:
                result += F("Wednesday");
                break;
            case THU:
                result += F("Thursday");
                break;
            case FRI:
                result += F("Friday");
                break;
            case SAT:
                result += F("Saturday");
                break;
            case SUN:
                result += F("Sunday");
                break;
            case 0:
                result += F("N/A");
                break;
        }
    }
    if (date) {
        if (day) result += F(" ");

        result += clock.dayOfMonth;
        result += F("-");
        result += clock.month;
        result += F("-");
        result += clock.year;

        if (hour) result += F(" ");
    }
    if (hour) {
        result += clock.hour;
        result += F("h");
        result += clock.minute;
        result += F("m");
        result += clock.second;
        result += F("s");
    }
    return result;
}

String getBME() {
    float temp(NAN), hum(NAN), pres(NAN);
    bme.read(pres, temp, hum, BME280::TempUnit_Celsius, BME280::PresUnit_hPa);
    String result = "";

    if (isnan(temp) || isnan(hum) || isnan(pres)) {
        serial.println(F("Failed to read BME!"));
        result += F("0,0,0");
    }
    else {
        result += String(temp);
        result += F(",");
        result += String(hum);
        result += F(",");
        result += String(pres);
    }
    return result;
}

void buttonPressed(Button button, unsigned short pressDuration) {
    switch (mode) {
        case STANDARD_MODE:
            if (button.pin == buttons[0].pin) { //TODO: Remove this temp code
                serial.println(F("Config mode"));
                mode = CONFIG_MODE;
                setLedState(LED_CONFIG_MODE);
            } else if (button.pin == buttons[1].pin && pressDuration >= 5000) {
                serial.println(F("Maintenance mode"));
                mode = MAINTAIN_MODE;
                setLedState(LED_MAINTAIN_MODE);
                closeSD();
            }
            break;
        case ECO_MODE:
            break;
        case CONFIG_MODE:
            break;
        case MAINTAIN_MODE:
            if (button.pin == buttons[1].pin && pressDuration >= 5000) {
                serial.println(F("Standard mode"));
                mode = STANDARD_MODE;
                setLedState(LED_STANDARD_MODE);
                initSD();
            }
            break;
    }
}

void logConfig() {
    /*serial.println(F("- Config -"));

    serial.print(F("Misc - Log interval: "));
    serial.print(config.logIntervalMin);
    serial.print(F(", Timout: "));
    serial.print(config.timeoutSec);
    serial.print(F(", Max file size: "));
    serial.println(config.fileMaxSizeKo);

    serial.print(F("Luminosity - Enable: "));
    serial.print(bool(config.lumSensorEnable) ? "true" : "false");
    serial.print(F(", Low: "));
    serial.print(config.lumSensorLow);
    serial.print(F(", High: "));
    serial.println(config.lumSensorHigh);

    serial.print(F("Temperature - Enable: "));
    serial.print(bool(config.tempSensorEnable) ? "true" : "false");
    serial.print(F(", Low: "));
    serial.print(config.tempSensorLow);
    serial.print(F(", High: "));
    serial.println(config.tempSensorHigh);

    serial.println(F("- End -"));*/
}