#include "Arduino.h"
#include "data.cpp"

static void buttonPressed(Button button) {
    Serial.println("Button " + String(button.pin) + " pressed");
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

