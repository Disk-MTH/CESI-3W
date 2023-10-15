#include "headers/data.h"
#include "headers/modes.h"
#include "headers/misc.h"
#include "EEPROM.h"
#include "Wire.h"

void setup() {
    serial.begin(9600);
    while (!serial);
    serial.println(F("### 3W initialization ###"));

    byte isConfigured;
    EEPROM.get(0, isConfigured);

    if (isConfigured == 135) {
        serial.println(F("A config exist in EEPROM, loading..."));
        EEPROM.get(1, config);
    } else {
        serial.println(F("No config in EEPROM, creating default..."));
        EEPROM.put(0, 135);
        EEPROM.put(1, config);
    }

    logConfig();
    Wire.begin();

    serial.print(F("Initializing BUTTONS..."));
    for (auto & button : buttons)
        pinMode(button.pin, INPUT);
    serial.println(F("Done!"));

    initSD();

    serial.print(F("Initializing LED..."));
    led.init();
    serial.println(F("Done!"));

    serial.print(F("Initializing RTC..."));
    clock.begin();
    clock.setTime();
    serial.println(F("Done!"));

    bme.begin();
    serial.print(F("Initializing Bme280..."));
    if (!bme.begin())
        serial.println(F("Failed!"));
    else {
        isBmeInit = true;
        serial.println(F("Done!"));
    }

    serial.println(F("### Initialization done ###"));

    if (digitalRead(buttons[1].pin) == LOW) {
        serial.println(F("Entering config mode..."));
        mode = CONFIG_MODE;
        setLedState(LED_CONFIG_MODE);
    }
}

void loop() {
    if (millis() - lastMillisTick > 0) {
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
                button.pressDuration++;
            else if (button.pressDuration > 0) {
                buttonPressed(button, button.pressDuration);
                button.pressDuration = 0;
            }
        }

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