#include "Arduino.h"
#include "EEPROM.h"
#include "data.cpp"
#include "modes.cpp"

void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("### 3W initialization ###");

    byte isConfigured;
    EEPROM.get(0, isConfigured);

    if (isConfigured == 136) {
        Serial.println("A config exist in EEPROM, loading...");
        EEPROM.get(1, config);
    } else {
        Serial.println("No config in EEPROM, creating default...");
        EEPROM.put(0, 136);
        EEPROM.put(1, config);
    }

    Serial.println("--- Config loaded ---");
    Serial.println("Misc: " + String(config.logIntervalMin) + ", " + String(config.timeoutSec) + ", " + String(config.fileMaxSizeKo));
    Serial.println("Luminosity sensor: " + String(bool(config.lumSensorEnable)) + ", Low: " + String(config.lumSensorLow) + ", High: " + String(config.lumSensorHigh));
    Serial.println("Temperature sensor: " + String(bool(config.tempSensorEnable)) + ", Low: " + String(config.tempSensorLow) + ", High: " + String(config.tempSensorHigh));
    Serial.println("Humidity sensor: " + String(bool(config.humSensorEnable)) + ", Low: " + String(config.humSensorLow) + ", High: " + String(config.humSensorHigh));
    Serial.println("--- End of config ---");

    led.init();

    Serial.println("### Initialization done ###");
}

void loop() {
    switch (currentMode) {
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
        if (ledStateData[currentState].timeLeft == 0) {
            led.setColorRGB(0, ledStateData[currentState].colors[ledStateData[currentState].colorIndex].red,
                            ledStateData[currentState].colors[ledStateData[currentState].colorIndex].green,
                            ledStateData[currentState].colors[ledStateData[currentState].colorIndex].blue);

            ledStateData[currentState].timeLeft = ledStateData[currentState].colors[ledStateData[currentState].colorIndex].durationSec;

            if (ledStateData[currentState].colorIndex == ledStateData[currentState].colorCount - 1)
                ledStateData[currentState].colorIndex = 0;
            else
                ledStateData[currentState].colorIndex++;
        }
        ledStateData[currentState].timeLeft--;
        lastLedTick = millis();
    }
}

