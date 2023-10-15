#include "headers/data.h"
#include "headers/modes.h"
#include "headers/misc.h"
#include "EEPROM.h"
#include "Wire.h"

void setup() {
    minSerial.begin(9600);
    while (!minSerial);
    minSerial.println(F("### 3W initialization ###"));

    byte isConfigured;
    EEPROM.get(0, isConfigured);

    if (isConfigured == 136) {
        minSerial.println(F("A config exist in EEPROM, loading..."));
        EEPROM.get(1, config);
    } else {
        minSerial.println(F("No config in EEPROM, creating default..."));
        EEPROM.put(0, 136);
        EEPROM.put(1, config);
    }

    logConfig();
    Wire.begin();

    minSerial.print(F("Initializing BUTTONS..."));
    for (auto & button : buttons)
        pinMode(button.pin, INPUT);
    minSerial.println(F("Done!"));

    bool initSd = initSD();//TODO: Add error handling for sensors

    minSerial.print(F("Initializing LED..."));
    led.init();
    minSerial.println(F("Done!"));

    minSerial.print(F("Initializing RTC..."));
    clock.begin();
    clock.setTime();
    minSerial.println(F("Done!"));

    bme.begin();
    minSerial.print(F("Initializing Bme280..."));
    if (!bme.begin()) //TODO: Add error handling for sensors
        minSerial.println(F("Failed!"));
    else
        minSerial.println(F("Done!"));

    minSerial.println(F("### Initialization done ###"));

    if (digitalRead(buttons[1].pin) == LOW) {
        minSerial.println(F("Entering config mode..."));
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

        clock.getTime();
        lastMillisTick = millis();
    }
}