#include "h/modes.h"
#include "h/data.h"
#include "h/misc.h"
#include "EEPROM.h"

void standardMode(bool eco) {
    if (ledState != LED_STANDARD_MODE && ledState != LED_ECO_MODE)
        return;

    const unsigned long delta = millis()  - lastMillisLog;
    const unsigned long duration = config.logIntervalMin * 60000 * (eco ? 2 : 1);

    if (delta > duration - 250) {
        if (gpsSerial.available() > 0 && gps.length() == 0) {
            const String data = gpsSerial.readStringUntil('\n');
            if (data.startsWith("$GPGGA"))
                gps = data;
        }
    }
    if (delta > duration) {
        const String date = getRTC(true, false);
        String fileName = date;
        fileName += "0.csv";

        if (logFile.open(fileName.c_str(), FILE_WRITE)) {
            if (logFile.fileSize() == 0)
                logFile.println(F("hour;temp;hum;pressure;luminosity;gps"));

            logData(logFile);

            if (gps.length() == 0 && (!eco || shouldLogGps)) {
                Serial.println(F("Invalid GPS data!"));
                setLedState(LED_INVALID_SENSOR_DATA);
            }

            if (eco)
                shouldLogGps = !shouldLogGps;
            else
                shouldLogGps = true;

            if (logFile.fileSize() > config.fileMaxSizeKo * 1024) {
                int index = 1;
                String newFileName = date;
                newFileName += index;
                newFileName += ".csv";

                while (sd.exists(newFileName.c_str())) {
                    index++;
                    newFileName = date;
                    newFileName += index;
                    newFileName += ".csv";
                }

                Serial.print(F("Archiving to "));
                Serial.print(newFileName);
                Serial.print(F("..."));

                if (logFile.rename(newFileName.c_str()))
                    Serial.println(DONE);
                else {
                    Serial.println(FAILED);
                    setLedState(LED_SD_ERROR);
                }
            }
            logFile.close();
        } else {
            Serial.println(F("Failed to open log file!"));
            setLedState(LED_SD_ERROR);
        }

        lastMillisLog = millis();
    }
}

void configMode() {
    if (configAfkCount > 1800000) {
        Serial.println(F("AFK for 30 minutes, exiting config mode"));
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

        if (command.startsWith((F("EXIT")))) {
            Serial.println(F("Exiting config mode"));
            mode = STANDARD_MODE;
            setLedState(LED_STANDARD_MODE);
            isConfigCommand = false;
        } else if (command.startsWith(F("VERSION"))) {
            Serial.print(F("3W v"));
            Serial.println(VERSION);
            Serial.print(F("Lot id: "));
            Serial.println(LOT_ID);
            isConfigCommand = false;
        } else if (command.startsWith((F("RESET")))) {
            Serial.println(F("Resetting config"));
            config = Config();
        } else if (command.startsWith((F("LOG_INTERVAL=")))) {
            const byte value = (byte) command.substring(13).toInt();
            if (value > 0)
                config.logIntervalMin = (byte) value;
            else
                invalidValue = true;
        } else if (command.startsWith(F("FILE_MAX_SIZE="))) {
            const short value = (short) command.substring(14).toInt();
            if (value > 0 && value < SHRT_MAX)
                config.fileMaxSizeKo = (short) value;
            else
                invalidValue = true;
        } else if (command.startsWith(F("LUMIN="))) {
            const byte value = (byte) command.substring(6).toInt();
            if (value >= 0 && value <= 1)
                config.lumSensorEnable = (bool) value;
            else
                invalidValue = true;
        } else if (command.startsWith((F("LUMIN_LOW=")))) {
            const short value = (short) command.substring(10).toInt();
            if (value > 0 && value < 1024)
                config.lumSensorLow = (short) value;
            else
                invalidValue = true;
        } else if (command.startsWith((F("LUMIN_HIGH=")))) {
            const short value = (short) command.substring(11).toInt();
            if (value > 0 && value < 1024)
                config.lumSensorHigh = (short) value;
            else
                invalidValue = true;
        } else if (command.startsWith(F("TEMP_AIR="))) {
            const byte value = (byte) command.substring(9).toInt();
            if (value >= 0 && value <= 1)
                config.tempSensorEnable = (bool) value;
            else
                invalidValue = true;
        } else if (command.startsWith(F("MIN_TEMP_AIR="))) {
            const short value = (short) command.substring(13).toInt();
            if (value > -39 && value < 86)
                config.tempSensorLow = (short) value;
            else
                invalidValue = true;
        } else if (command.startsWith(F("MAX_TEMP_AIR="))) {
            const short value = (short) command.substring(13).toInt();
            if (value > -39 && value < 86)
                config.tempSensorHigh = (short) value;
            else
                invalidValue = true;
        } else if (command.startsWith(F("HYGR="))) {
            const byte value = (byte) command.substring(5).toInt();
            if (value >= 0 && value <= 1)
                config.humSensorEnable = (bool) value;
            else
                invalidValue = true;
        } else if (command.startsWith(F("HYGR_MIN="))) {
            const short value = (short) command.substring(9).toInt();
            if (value > -39 && value < 86)
                config.humSensorLow = (short) value;
            else
                invalidValue = true;
        } else if (command.startsWith(F("HYGR_MAX="))) {
            const short value = (short) command.substring(9).toInt();
            if (value > -39 && value < 86)
                config.humSensorHigh = (short) value;
            else
                invalidValue = true;
        } else if (command.startsWith(F("PRESSURE="))) {
            const byte value = (byte) command.substring(9).toInt();
            if (value >= 0 && value <= 1)
                config.presSensorEnable = (bool) value;
            else
                invalidValue = true;
        } else if (command.startsWith(F("PRESSURE_MIN="))) {
            const short value = (short) command.substring(13).toInt();
            if (value > 299 && value < 1101)
                config.pressSensorLow = (short) value;
            else
                invalidValue = true;
        } else if (command.startsWith(F("PRESSURE_MAX="))) {
            const short value = (short) command.substring(13).toInt();
            if (value > 299 && value < 1101)
                config.pressSensorHigh = (short) value;
            else
                invalidValue = true;
        } else if (command.startsWith(F("DATE="))) {
            const byte day = (byte) command.substring(5, 7).toInt();
            const byte month = (byte) command.substring(7, 9).toInt();
            const short year = (short) command.substring(9, 13).toInt();
            if (day > 0 && day < 32 && month > 0 && month < 13 && year > 1999 && year < 2100) {
                clock.fillByYMD(year, month, day);
                clock.setTime();
                Serial.print(F("Date set to: "));
                Serial.println(getRTC(true, false));
            } else
                invalidValue = true;
            isConfigCommand = false;
        }
        else if (command.startsWith((F("CLOCK=")))) {
            const byte hour = (byte) command.substring(6, 8).toInt();
            const byte minute = (byte) command.substring(8, 10).toInt();
            const byte second = (byte) command.substring(10, 12).toInt();
            if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60) {
                clock.fillByHMS(hour, minute, second);
                clock.setTime();
                Serial.print(F("Time set to: "));
                Serial.println(getRTC(false, true));
            } else
                invalidValue = true;
            isConfigCommand = false;
        } else
            invalidSyntax = true;
        if (isConfigCommand && !invalidSyntax && !invalidValue) {
            EEPROM.update(0, EEPROM_FLAG);
            EEPROM.put(1, config);
            logConfig();
        } else if (invalidSyntax)
            Serial.println(F("Invalid syntax!"));
        else if (invalidValue)
            Serial.println(F("Invalid value!"));
    }
}

void maintainMode() {
    if (millis() - lastMillisLog > config.logIntervalMin * 60000) {
        logData(Serial);
        lastMillisLog = millis();
    }
}