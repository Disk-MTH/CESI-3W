#include "headers/misc.h"
#include "headers/data.h"

void initSD() {
    if (isSdInit) return;

    while (!isSdInit) {
        serial.print(F("Initializing SD card..."));
        if (!sd.begin(4)) {
            isSdInit = false;
            serial.println(F("Failed!"));
            delay(5000);
        } else {
            isSdInit = true;
            serial.println(F("Done!"));
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

void readBme280() {
    float temp(NAN), hum(NAN), pres(NAN);
    bme.read(pres, temp, hum, BME280::TempUnit_Celsius, BME280::PresUnit_hPa);

    if (isnan(temp))
        serial.println(F("Failed to read temperature!"));
    else if (isnan(hum))
        serial.println(F("Failed to read humidity!"));
    else if (isnan(pres))
        serial.println(F("Failed to read pressure!"));
    else {
        serial.print(F("Temperature: "));
        serial.print(temp);
        serial.print(F(" °C, Humidity: "));
        serial.print(hum);
        serial.print(F(" %, Pressure: "));
        serial.print(pres);
        serial.println(F(" hPa"));
    }
}

void buttonPressed(Button button, unsigned short pressDuration) {
    switch (mode) {
        case STANDARD_MODE:
            if (button.pin == buttons[0].pin) { //TODO: Remove this temp code
                serial.println(F("Entering config mode..."));
                mode = CONFIG_MODE;
                setLedState(LED_CONFIG_MODE);
            } else if (button.pin == buttons[1].pin && pressDuration >= 5000) {
                serial.println(F("Entering maintenance mode..."));
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
                serial.println(F("Entering standard mode..."));
                mode = STANDARD_MODE;
                setLedState(LED_STANDARD_MODE);
                initSD();
            }
            break;
    }
}

void logConfig() {
    serial.println(F("--- Config ---"));

    serial.print(F("Misc - Log interval: "));
    serial.print(config.logIntervalMin);
    serial.print(F(", Timout: "));
    serial.print(config.timeoutSec);
    serial.print(F(", Max file size: "));
    serial.println(config.fileMaxSizeKo);

    serial.print(F("Luminosity sensor - Enable: "));
    serial.print(bool(config.lumSensorEnable) ? "true" : "false");
    serial.print(F(", Low: "));
    serial.print(config.lumSensorLow);
    serial.print(F(", High: "));
    serial.println(config.lumSensorHigh);

    serial.print(F("Temperature sensor - Enable: "));
    serial.print(bool(config.tempSensorEnable) ? "true" : "false");
    serial.print(F(", Low: "));
    serial.print(config.tempSensorLow);
    serial.print(F(", High: "));
    serial.println(config.tempSensorHigh);

    serial.println(F("--- End of config ---"));
}

String getFormattedDate(bool day, bool date, bool hour) {
    clock.getTime();
    String formattedDate = "";
    if (day) {
        switch (clock.dayOfWeek) {
            case MON:
                formattedDate += F("Monday");
                break;
            case TUE:
                formattedDate += F("Tuesday");
                break;
            case WED:
                formattedDate += F("Wednesday");
                break;
            case THU:
                formattedDate += F("Thursday");
                break;
            case FRI:
                formattedDate += F("Friday");
                break;
            case SAT:
                formattedDate += F("Saturday");
                break;
            case SUN:
                formattedDate += F("Sunday");
                break;
            case 0:
                formattedDate += F("N/A");
                break;
        }
    }
    if (date) {
        if (day) formattedDate += F(" ");

        formattedDate += clock.dayOfMonth;
        formattedDate += F("-");
        formattedDate += clock.month;
        formattedDate += F("-");
        formattedDate += clock.year + 2000;

        if (hour) formattedDate += F(" ");
    }
    if (hour) {
        formattedDate += clock.hour;
        formattedDate += F("h");
        formattedDate += clock.minute;
        formattedDate += F("m");
        formattedDate += clock.second;
        formattedDate += F("s");
    }
    return formattedDate;
}