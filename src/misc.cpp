#include "Arduino.h"
#include "data.cpp"

static void buttonPressed(Button button) {
    switch (mode) {
        case STANDARD_MODE:
            Serial.println(F("Entering config mode..."));
            mode = CONFIG_MODE;
            ledState = LED_CONFIG_MODE;
            break;
        case ECO_MODE:
            break;
        case CONFIG_MODE:
            break;
        case MAINTAIN_MODE:
            break;
    }
}

static void logConfig() {
    Serial.println(F("--- Config ---"));
    Serial.println("Misc: " + String(config.logIntervalMin) + ", " + String(config.timeoutSec) + ", " + String(config.fileMaxSizeO));
    Serial.println("Luminosity sensor - Enable: " + String(bool(config.lumSensorEnable) ? "true" : "false") + ", Low: " + String(config.lumSensorLow) + ", High: " + String(config.lumSensorHigh));
    Serial.println("Temperature sensor - Enable: " + String(bool(config.tempSensorEnable) ? "true" : "false") + ", Low: " + String(config.tempSensorLow) + ", High: " + String(config.tempSensorHigh));
    Serial.println("Humidity sensor - Enable: " + String(bool(config.humSensorEnable) ? "true" : "false") + ", Low: " + String(config.humSensorLow) + ", High: " + String(config.humSensorHigh));
    Serial.println(F("--- End of config ---"));
}

static void setLedState(LedState state) {
    ledStateData[ledState].colorIndex = 0;
    ledStateData[ledState].millisLeft = 0;
    ledState = state;
    ledStateData[ledState].colorIndex = 0;
    ledStateData[ledState].millisLeft = 0;
}

static void readBme280() {
    float temp(NAN), hum(NAN), pres(NAN);
    bme.read(pres, temp, hum, BME280::TempUnit_Celsius, BME280::PresUnit_hPa);

    if (isnan(temp))
        Serial.println("Failed to read temperature!");
    else if (isnan(hum))
        Serial.println("Failed to read humidity!");
    else if (isnan(pres))
        Serial.println("Failed to read pressure!");
    else
        Serial.println("Temperature: " + String(temp) + " °C, " + "Humidity: " + String(hum) + " %, " + "Pressure: " + String(pres) + " hPa");
}