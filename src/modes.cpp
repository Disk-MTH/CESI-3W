#include "Arduino.h"
#include "EEPROM.h"
#include "limits.h"
#include "misc.cpp"


static unsigned long lastlog = 0;

static void standardMode() {
    //readBme280();
    if (millis() - lastlog > 1000) {
        logClock(true, true, true);
        lastlog = millis();
    }
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
        Serial.println(command);
        configAfkCount = 0;
        askForPrompt = true;
        bool isConfigCommand = true;
        bool invalidValue = false;
        bool invalidSyntax = false;

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
        } else if (command.startsWith(("LOG_INTERVAL="))) {
                const long value = command.substring(13).toInt();
                if (value > 0 && value < 256)
                    config.logIntervalMin = value;
                else
                    invalidValue = true;
        } else if (command.startsWith(F("TIMEOUT="))) {
            const long value = command.substring(8).toInt();
            if (value > 0 && value < 256)
                config.timeoutSec = value;
            else
                invalidValue = true;
        } else if (command.startsWith(F("FILE_MAX_SIZE="))) {
            const long value = command.substring(14).toInt();
            if (value > 0 && value < INT_MAX)
                config.fileMaxSizeO = (int) value;
            else
                invalidValue = true;
        } else if (command.startsWith(F("LUMIN="))) {
            const long value = command.substring(6).toInt();
            if (value >= 0 && value <= 1)
                config.lumSensorEnable = (bool) value;
            else
                invalidValue = true;
        } else if (command.startsWith(("LUMIN_LOW="))) {
            const long value = command.substring(10).toInt();
            if (value > 0 && value < 1024)
                config.lumSensorLow = (int) value;
            else
                invalidValue = true;
        } else if (command.startsWith(("LUMIN_HIGH="))) {
            const long value = command.substring(11).toInt();
            if (value > 0 && value < 1024)
                config.lumSensorHigh = (int) value;
            else
                invalidValue = true;
        } else if (command == F("TEMP_AIR=")) {

        } else if (command == F("MIN_TEMP_AIR=")) {

        } else if (command == F("MAX_TEMP_AIR=")) {

        } else if (command == F("HYGR=")) {

        } else if (command == F("HYGR_MIN=")) {

        } else if (command == F("HYGR_MAX=")) {

        } else if (command == F("PRESSURE=")) {

        } else if (command == F("PRESSURE_MIN=")) {

        } else if (command == F("PRESSURE_MAX=")) {

        } else if (command.startsWith(("DATE="))) {
            const long day = command.substring(5, 7).toInt();
            const long month = command.substring(7, 9).toInt();
            const long year = command.substring(9, 13).toInt();
            if (day > 0 && day < 32 && month > 0 && month < 13 && year > 1999 && year < 3000) {
                clock.fillByYMD(year, month, day);
                clock.setTime();
                Serial.print(F("Date set to: "));
                logClock(true, false, false);
            } else
                invalidValue = true;
            isConfigCommand = false;
        } else if (command.startsWith(("DAY="))) {
            const String day = command.substring(4);

            if (day.startsWith("MON"))
                clock.dayOfWeek = MON;
            else if (day.startsWith("TUE"))
                clock.dayOfWeek = TUE;
            else if (day.startsWith("WED"))
                clock.dayOfWeek = WED;
            else if (day.startsWith("THU"))
                clock.dayOfWeek = THU;
            else if (day.startsWith("FRI"))
                clock.dayOfWeek = FRI;
            else if (day.startsWith("SAT"))
                clock.dayOfWeek = SAT;
            else if (day.startsWith("SUN"))
                clock.dayOfWeek = SUN;
            else
                invalidValue = true;

            if (!invalidValue) {
                clock.setTime();
                Serial.print(F("Day set to: "));
                logClock(false, true, false);
            }
            isConfigCommand = false;
        } else if (command == F("CLOCK=")) {

        } else
            invalidSyntax = true;

        if (isConfigCommand && !invalidSyntax && !invalidValue) {
            EEPROM.update(0, 136);
            EEPROM.put(1, config);
            logConfig();
        } else if (invalidSyntax)
            Serial.println(F("Invalid command syntax! Check the documentation"));
        else if (invalidValue)
            Serial.println(F("Invalid value! Check the documentation"));
    }
}

static void maintainMode() {

}