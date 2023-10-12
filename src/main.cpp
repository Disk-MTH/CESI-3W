#include "Arduino.h"
#include "EEPROM.h"

enum Mode {
    STANDARD_MODE,
    CONFIG_MODE,
    ECO_MODE,
    REPAIR_MODE,
};

struct Config {
    byte logIntervalMin = 10;
    byte timeoutSec = 10;
    byte fileMaxSizeKo = 2;
    bool lumSensorEnable = true;
    int lumSensorLow = 255;
    int lumSensorHigh = 768;
    bool tempSensorEnable = true;
    int tempSensorLow = -10;
    int tempSensorHigh = 60;
    bool humSensorEnable = true;
    int humSensorLow = 0;
    int humSensorHigh = 50;
};

Mode currentMode = STANDARD_MODE;
Config config = Config();

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

    Serial.println("### Initialization done ###");
}

void loop() {

}

void standardMode() {

}

void configMode() {

}

void ecoMode() {

}