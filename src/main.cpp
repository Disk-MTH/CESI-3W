#include "Arduino.h"
#include "EEPROM.h"
#include "Wire.h"
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

    logConfig();
    Wire.begin();

    Serial.print(F("Initializing BUTTONS..."));
    for (auto & button : buttons)
        pinMode(button.pin, INPUT);
    Serial.println(F("Done!"));

    Serial.print(F("Initializing LED..."));
    led.init();
    Serial.println(F("Done!"));

    Serial.print(F("Initializing Bme280..."));
    if (!bme.begin())
        Serial.println(F("Failed!"));
    Serial.println(F("Done!"));

    Serial.println(F("### Initialization done ###"));

    if (digitalRead(buttons[1].pin) == LOW) {
        Serial.println(F("Entering config mode..."));
        mode = CONFIG_MODE;
        ledState = LED_CONFIG_MODE;
    }
}

void loop() {
    if (millis() - lastMillisTick > 0) {
        everyMillis();
        switch (mode) {
            case STANDARD_MODE:
                standardMode();
                break;
            case ECO_MODE:
                ecoMode();
                break;
            case CONFIG_MODE:
                configMode();
                break;
            case MAINTAIN_MODE:
                maintainMode();
                break;
        }
        lastMillisTick = millis();
    }
}