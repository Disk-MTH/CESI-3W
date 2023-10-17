#include "headers/data.h"
#include "headers/modes.h"
#include "headers/misc.h"
#include "EEPROM.h"
#include "Wire.h"

void setup() {
    serial.begin(9600);
    gpsSerial.begin(9600);
    gpsSerial.listen();
    while (!serial);
    serial.println(F("### 3W ###"));

    byte isConfigured;
    EEPROM.get(0, isConfigured);

    if (isConfigured == 135) {
        serial.println(F("Load config from EEPROM..."));
        EEPROM.get(1, config);
    } else {
        serial.println(F("Create default config..."));
        EEPROM.put(0, 135);
        EEPROM.put(1, config);
    }

    logConfig();
    Wire.begin();

    serial.print(F("BUTTONS..."));
    for (auto &button: buttons)
        pinMode(button.pin, INPUT);
    serial.println(DONE);

    initSD();

    serial.print(F("LED..."));
    led.init();
    serial.println(DONE);

    serial.print(F("RTC..."));
    clock.begin();
    clock.setTime();
    serial.println(DONE);

    serial.print(F("BME..."));
    if (!bme.begin())
        serial.println(FAILED);
    else {
        isBmeInit = true;
        serial.println(DONE);
    }

    serial.println(F("### Finish ###"));

    if (digitalRead(buttons[1].pin) == LOW) {
        serial.println(F("Config mode"));
        mode = CONFIG_MODE;
        setLedState(LED_CONFIG_MODE);
    }
}

void loop() {
    if (gpsSerial.available() > 0 && gps.length() == 0) {
        const String data = gpsSerial.readStringUntil('\n');
        if (data.startsWith("$GPGGA"))
            gps = data;
    }

    if (millis() - lastMillisTick > 1000) {
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
                standardMode(false);
                break;
            case ECO_MODE:
                standardMode(true);
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