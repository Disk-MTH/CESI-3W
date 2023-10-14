#include "Arduino.h"
#include "EEPROM.h"
#include "limits.h"
#include "misc.cpp"

static void standardMode() {
    //readBme280();
}

static void ecoMode() {

}

static void configMode() {
    if (configAfkCount > 1800000) {
        Serial.println(F("No activity for 30 minutes, exiting config mode..."));
        mode = STANDARD_MODE;
        setLedState(LED_STANDARD_MODE);
        configAfkCount = 0;
        askForPrompt = true;
        return;
    }

    configAfkCount++;

    if (askForPrompt) {
        while (Serial.available() > 0) Serial.read();
        Serial.println(F("Enter a command:"));
        askForPrompt = false;
    }

    if (Serial.available() > 0) {
        const String command = Serial.readStringUntil('\n');
        configAfkCount = 0;
        askForPrompt = true;
        bool isConfigCommand = true;
        bool invalidValueRange = false;
        bool invalidCommandSyntax = false;

        if (command.startsWith(("EXIT"))) {
            Serial.println(F("Exiting config mode..."));
            mode = STANDARD_MODE;
            setLedState(LED_STANDARD_MODE);
            isConfigCommand = false;
        } else if (command.startsWith(F("VERSION"))) {
            Serial.println(F("3W v1.0.0"));
            isConfigCommand = false;
        } else if (command.startsWith(("RESET"))) {
            Serial.println(F("Resetting config..."));
            config = Config();
        } else if (command.startsWith(("LOG_INTERVAL=")))
            if (command.length() > 13) {
                const long value = command.substring(13).toInt();
                if (value > 0 && value < 256)
                    config.logIntervalMin = value;
                else
                    invalidValueRange = true;
            } else
                invalidCommandSyntax = true;
        else if (command.startsWith(F("TIMEOUT=")))
            if (command.length() > 8) {
                const long value = command.substring(9).toInt();
                if (value > 0 && value < 256)
                    config.timeoutSec = value;
                else
                    invalidValueRange = true;
            } else
                invalidCommandSyntax = true;
        else if (command.startsWith(F("FILE_MAX_SIZE=")))
            if (command.length() > 14) {
                const long value = command.substring(15).toInt();
                if (value > 0 && value < INT_MAX)
                    config.fileMaxSizeO = (int) value;
                else
                    invalidValueRange = true;
            } else
                invalidCommandSyntax = true;
        else if (command.startsWith(F("LUMIN=")))
            if (command.length() > 6) {
                const long value = command.substring(6).toInt();
                if (value >= 0 && value <= 1)
                    config.lumSensorEnable = (bool) value;
                else
                    invalidValueRange = true;
            } else
                invalidCommandSyntax = true;
        else if (command.startsWith(("LUMIN_LOW=")))
            if (command.length() > 10) {
                const long value = command.substring(10).toInt();
                if (value > 0 && value < 1024)
                    config.lumSensorLow = (int) value;
                else
                    invalidValueRange = true;
            } else
                invalidCommandSyntax = true;
        else if (command.startsWith(("LUMIN_HIGH=")))
            if (command.length() > 11) {
                const long value = command.substring(11).toInt();
                if (value > 0 && value < 1024)
                    config.lumSensorHigh = (int) value;
                else
                    invalidValueRange = true;
            } else
                invalidCommandSyntax = true;
        else if (command == F("TEMP_AIR=")) {

        } else if (command == F("MIN_TEMP_AIR=")) {

        } else if (command == F("MAX_TEMP_AIR=")) {

        } else if (command == F("HYGR=")) {

        } else if (command == F("HYGR_MIN=")) {

        } else if (command == F("HYGR_MAX=")) {

        } else if (command == F("PRESSURE=")) {

        } else if (command == F("PRESSURE_MIN=")) {

        } else if (command == F("PRESSURE_MAX=")) {

        } else if (command == F("DATE=")) {

        } else if (command == F("DAY=")) {

        } else if (command == F("CLOCK=")) {

        } else
            invalidCommandSyntax = true;

        if (isConfigCommand &&!invalidCommandSyntax && !invalidValueRange) {
            EEPROM.update(0, 136);
            EEPROM.put(1, config);
            logConfig();
        } else if (invalidCommandSyntax)
            Serial.println(F("Invalid command syntax! Check the documentation"));
        else if (invalidValueRange)
            Serial.println(F("Invalid value range! Check the documentation"));
    }
}

static void maintainMode() {

}