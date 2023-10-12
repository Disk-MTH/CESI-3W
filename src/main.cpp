#include "Arduino.h"
#include "EEPROM.h"
#include "data.cpp"
#include "modes.cpp"

void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println(F("### 3W initialization ###"));

    byte isConfigured;
    EEPROM.get(0, isConfigured);

    if (isConfigured == 136) {
        Serial.println(F("A config exist in EEPROM, loading..."));
        EEPROM.get(1, config);
    } else {
        Serial.println(F("No config in EEPROM, creating default..."));
        EEPROM.put(0, 136);
        EEPROM.put(1, config);
    }

    Serial.println(F("--- Config ---"));
    Serial.println("Misc: " + String(config.logIntervalMin) + ", " + String(config.timeoutSec) + ", " + String(config.fileMaxSizeKo));
    Serial.println("Luminosity sensor: " + String(bool(config.lumSensorEnable)) + ", Low: " + String(config.lumSensorLow) + ", High: " + String(config.lumSensorHigh));
    Serial.println("Temperature sensor: " + String(bool(config.tempSensorEnable)) + ", Low: " + String(config.tempSensorLow) + ", High: " + String(config.tempSensorHigh));
    Serial.println("Humidity sensor: " + String(bool(config.humSensorEnable)) + ", Low: " + String(config.humSensorLow) + ", High: " + String(config.humSensorHigh));
    Serial.println(F("--- End of config ---"));

    Serial.print(F("Initializing LED..."));
    led.init();
    Serial.println(F("Done!"));

    Serial.println(F("### Initialization done ###"));
}

void loop() {
    switch (mode) {
        case STANDARD_MODE:
            standardMode();
            break;
        case ECO_MODE:
            configMode();
            break;
        case CONFIG_MODE:
            ecoMode();
            break;
        case MAINTAIN_MODE:
            maintainMode();
            break;
    }

    if (millis() - lastLedTick > 1000) {
        if (ledStateData[ledState].timeLeft == 0) {
            led.setColorRGB(0, ledStateData[ledState].colors[ledStateData[ledState].colorIndex].red,
                            ledStateData[ledState].colors[ledStateData[ledState].colorIndex].green,
                            ledStateData[ledState].colors[ledStateData[ledState].colorIndex].blue);

            ledStateData[ledState].timeLeft = ledStateData[ledState].colors[ledStateData[ledState].colorIndex].durationSec;

            if (ledStateData[ledState].colorIndex == ledStateData[ledState].colorCount - 1)
                ledStateData[ledState].colorIndex = 0;
            else
                ledStateData[ledState].colorIndex++;
        }
        ledStateData[ledState].timeLeft--;
        lastLedTick = millis();
    }
}

