#include "h/misc.h"
#include "h/data.h"

void initSD() {
    if (isSdInit) return;

    while (!isSdInit) {
        Serial.print(F("SD card..."));
        if (!sd.begin(4)) {
            isSdInit = false;
            Serial.println(FAILED);
            delay(5000);
        } else {
            isSdInit = true;
            Serial.println(DONE);
        }
    }
}

void closeSD() {
    if (!isSdInit) return;

    sd.end();
    isSdInit = false;
    Serial.println(F("SD card closed!"));
}

void setLedState(LedState state) {
    ledStateData[ledState].colorIndex = 0;
    ledStateData[ledState].millisLeft = 0;
    ledState = state;
}

void logData(Print& outputStream) {
    outputStream.print(getRTC(false, true));
    outputStream.print(SEMI);
    outputStream.print(getBME(config.tempSensorEnable, config.humSensorEnable, config.presSensorEnable));
    outputStream.print(SEMI);
    outputStream.print(analogRead(A0));
    outputStream.print(SEMI);
    if (shouldLogGps)
        outputStream.print(gps);
    else
        outputStream.println(NA);
}

String getRTC(bool date, bool hour) {
    clock.getTime();
    String result = "";
    if (date) {
        result += clock.dayOfMonth;
        result += clock.month;
        result += (clock.year);

        if (hour) result += F(" ");
    }
    if (hour) {
        result += clock.hour;
        result += clock.minute;
        result += clock.second;
    }
    return result;
}

String getBME(bool temp, bool hum, bool pres) {
    float tempV(NAN), humV(NAN), presV(NAN);
    bme.read(presV, tempV, humV, BME280::TempUnit_Celsius, BME280::PresUnit_hPa);
    String result = "";

    if (isnan(tempV) || isnan(humV) || isnan(presV)) {
        Serial.println(F("Failed to read BME!"));
        setLedState(LED_SENSOR_ERROR);
    }

    if (!temp) {
        result += NA;
        result += SEMI;
    } else
        if (tempV < config.tempSensorLow || tempV > config.tempSensorHigh) {
            Serial.print(F("Invalid temperature! Range: "));
            Serial.print(config.tempSensorLow);
            Serial.print(F(" - "));
            Serial.print(config.tempSensorHigh);
            Serial.print(F(", value: "));
            Serial.println(tempV);
            setLedState(LED_INVALID_SENSOR_DATA);
        }
        else {
            result += String(tempV);
            result += SEMI;
        }

    if (!hum) {
        result += NA;
        result += SEMI;
    } else
        if (humV < config.humSensorLow || humV > config.humSensorHigh) {
            Serial.print(F("Invalid humidity! Range: "));
            Serial.print(config.humSensorLow);
            Serial.print(F(" - "));
            Serial.print(config.humSensorHigh);
            Serial.print(F(", value: "));
            Serial.println(humV);
            setLedState(LED_INVALID_SENSOR_DATA);
        }
        else {
            result += String(humV);
            result += SEMI;
        }

    if (!pres)
        result += NA;
    else
        if (presV < config.pressSensorLow || presV > config.pressSensorHigh) {
            Serial.print(F("Invalid pressure! Range: "));
            Serial.print(config.pressSensorLow);
            Serial.print(F(" - "));
            Serial.print(config.pressSensorHigh);
            Serial.print(F(", value: "));
            Serial.println(presV);
            setLedState(LED_INVALID_SENSOR_DATA);
        }
        else
            result += String(presV);

    return result;
}

void buttonPressed(Button button, unsigned short pressDuration) {
    switch (mode) {
        case STANDARD_MODE:
            if (button.pin == buttons[0].pin && pressDuration >= 5000) {
                Serial.println(F("Eco mode"));
                mode = ECO_MODE;
                setLedState(LED_ECO_MODE);
            } else if (button.pin == buttons[1].pin && pressDuration >= 5000) {
                Serial.println(F("Maintenance mode"));
                mode = MAINTAIN_MODE;
                setLedState(LED_MAINTAIN_MODE);
                closeSD();
            }
            break;
        case ECO_MODE:
            if (button.pin == buttons[1].pin && pressDuration >= 5000) {
                Serial.println(F("Standard mode"));
                mode = STANDARD_MODE;
                setLedState(LED_STANDARD_MODE);
            }
            break;
        case CONFIG_MODE:
            break;
        case MAINTAIN_MODE:
            if (button.pin == buttons[1].pin && pressDuration >= 5000) {
                Serial.println(F("Standard mode"));
                mode = STANDARD_MODE;
                setLedState(LED_STANDARD_MODE);
                initSD();
            }
            break;
    }
}

void logConfig() {
    Serial.println(F("- Config -"));

    Serial.print(F("Misc - Log interval: "));
    Serial.print(config.logIntervalMin);
    Serial.print(F(", Max file size: "));
    Serial.println(config.fileMaxSizeKo);

    Serial.print(F("Luminosity - Enable: "));
    Serial.print(bool(config.lumSensorEnable) ? "true" : "false");
    Serial.print(F(", Low: "));
    Serial.print(config.lumSensorLow);
    Serial.print(F(", High: "));
    Serial.println(config.lumSensorHigh);

    Serial.print(F("Temperature - Enable: "));
    Serial.print(bool(config.tempSensorEnable) ? "true" : "false");
    Serial.print(F(", Low: "));
    Serial.print(config.tempSensorLow);
    Serial.print(F(", High: "));
    Serial.println(config.tempSensorHigh);

    Serial.print(F("Humidity - Enable: "));
    Serial.print(bool(config.humSensorEnable) ? "true" : "false");
    Serial.print(F(", Low: "));
    Serial.print(config.humSensorLow);
    Serial.print(F(", High: "));
    Serial.println(config.humSensorHigh);

    Serial.print(F("Pressure - Enable: "));
    Serial.print(bool(config.presSensorEnable) ? "true" : "false");
    Serial.print(F(", Low: "));
    Serial.print(config.pressSensorLow);
    Serial.print(F(", High: "));
    Serial.println(config.pressSensorHigh);

    Serial.println(F("- End -"));
}