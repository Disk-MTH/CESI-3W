#include "Arduino.h"
#include "data.cpp"

static void buttonPressed(Button button) {
    //Serial.println("Button " + String(button.pin) + " pressed");
}

static void everyMillis() {
    if (ledStateData[ledState].millisLeft == 0) {
        led.setColorRGB(0, ledStateData[ledState].colors[ledStateData[ledState].colorIndex].red,
                        ledStateData[ledState].colors[ledStateData[ledState].colorIndex].green,
                        ledStateData[ledState].colors[ledStateData[ledState].colorIndex].blue);

        ledStateData[ledState].millisLeft = ledStateData[ledState].colors[ledStateData[ledState].colorIndex].durationMillis;

        if (ledStateData[ledState].colorIndex == ledStateData[ledState].colorCount - 1)
            ledStateData[ledState].colorIndex = 0;
        else
            ledStateData[ledState].colorIndex++;
    }
    ledStateData[ledState].millisLeft--;

    for (auto & button : buttons) {
        if (digitalRead(button.pin) == LOW)
            if (button.isPressed) {
                if (button.millisLeft == 0)
                    buttonPressed(button);
                button.millisLeft--;
            } else
                button.isPressed = true;
        else {
            button.isPressed = false;
            button.millisLeft = button.durationMillis;
        }
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