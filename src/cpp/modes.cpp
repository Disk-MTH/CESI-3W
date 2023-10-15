#include "headers/modes.h"
#include "headers/data.h"
#include "headers/misc.h"
#include "EEPROM.h"

void standardMode() {
    if (millis() - lastMillisLog > 1000/*config.logIntervalMin * 60000*/) {
        const String date = getFormattedDate(false, true, false);
        const String fileName = date + "_0.txt";
        if (logFile.open(fileName.c_str(), FILE_WRITE)) {
            if (logFile.fileSize() > 250/*config.fileMaxSizeKo * 1024*/) {
                int index = 1;
                while (sd.exists(date + "_" + index + ".txt"))
                    index++;

                const String newFileName = date + "_" + index + ".txt";

                serial.print(F("Log file is full, archiving to "));
                serial.print(newFileName);
                serial.println(F("..."));

                if (logFile.rename(newFileName.c_str()))
                    serial.println(F("Done!"));
                else
                    serial.println(F("Failed!"));
            }

            logFile.println(getFormattedDate(true, true, true));
            logFile.close();
        } else
            serial.println(F("Failed to open log file!"));

        lastMillisLog = millis();
    }
}

void ecoMode() {

}

void configMode() {
    if (configAfkCount > 1800000) {
        serial.println(F("No activity for 30 minutes, exiting config mode..."));
        mode = STANDARD_MODE;
        setLedState(LED_STANDARD_MODE);
        configAfkCount = 0;
        askForPrompt = true;
        return;
    }

    configAfkCount++;

    if (askForPrompt) {
        while (serial.available() > 0) serial.read();
        serial.println(F("Enter a command:"));
        askForPrompt = false;
    }

    if (serial.available() > 0) {
        String command;
        while (serial.available() > 0) {
            char c = (char) serial.read();
            if (c == '\n' || c == '\r')
                break;
            command += c;
        }

        serial.println(command);
        configAfkCount = 0;
        askForPrompt = true;
        bool isConfigCommand = true;
        bool invalidValue = false;
        bool invalidSyntax = false;

        if (command.startsWith((F("EXIT")))) {
            serial.println(F("Exiting config mode..."));
            mode = STANDARD_MODE;
            setLedState(LED_STANDARD_MODE);
            isConfigCommand = false;
        } else if (command.startsWith(F("VERSION"))) {
            serial.println(F("3W v1.0.0"));
            isConfigCommand = false;
        } else if (command.startsWith((F("RESET")))) {
            serial.println(F("Resetting config..."));
            config = Config();
        } else if (command.startsWith((F("LOG_INTERVAL=")))) {
                const byte value = (byte) command.substring(13).toInt();
                if (value > 0)
                    config.logIntervalMin = (byte) value;
                else
                    invalidValue = true;
        } else if (command.startsWith(F("TIMEOUT="))) {
            const byte value = (byte) command.substring(8).toInt();
            if (value > 0)
                config.timeoutSec = (byte) value;
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
        } else if (command == F("TEMP_AIR=")) {

        } else if (command == F("MIN_TEMP_AIR=")) {

        } else if (command == F("MAX_TEMP_AIR=")) {

        } else if (command == F("HYGR=")) {

        } else if (command == F("HYGR_MIN=")) {

        } else if (command == F("HYGR_MAX=")) {

        } else if (command == F("PRESSURE=")) {

        } else if (command == F("PRESSURE_MIN=")) {

        } else if (command == F("PRESSURE_MAX=")) {

        } else if (command.startsWith((F("DATE=")))) {
            const byte day = (byte) command.substring(5, 7).toInt();
            const byte month = (byte) command.substring(7, 9).toInt();
            const short year = (short) command.substring(9, 13).toInt();
            if (day > 0 && day < 32 && month > 0 && month < 13 && year > 1999 && year < 2100) {
                //clock.fillByYMD(year, month, day);
                //clock.setTime();
                serial.print(F("Date set to: "));
                serial.println(getFormattedDate(false, true, false));
            } else
                invalidValue = true;
            isConfigCommand = false;
        } else if (command.startsWith((F("DAY=")))) {
            const String day = command.substring(4);

            if (day.startsWith(F("MON")))
                clock.dayOfWeek = MON;
            else if (day.startsWith(F("TUE")))
                clock.dayOfWeek = TUE;
            else if (day.startsWith(F("WED")))
                clock.dayOfWeek = WED;
            else if (day.startsWith(F("THU")))
                clock.dayOfWeek = THU;
            else if (day.startsWith(F("FRI")))
                clock.dayOfWeek = FRI;
            else if (day.startsWith(F("SAT")))
                clock.dayOfWeek = SAT;
            else if (day.startsWith(F("SUN")))
                clock.dayOfWeek = SUN;
            else
                invalidValue = true;

            if (!invalidValue) {
                clock.setTime();
                serial.print(F("Day set to: "));
                serial.println(getFormattedDate(true, false, false));
            }
            isConfigCommand = false;
        } else if (command.startsWith((F("CLOCK=")))) {
            const byte hour = (byte) command.substring(6, 8).toInt();
            const byte minute = (byte) command.substring(8, 10).toInt();
            const byte second = (byte) command.substring(10, 12).toInt();
            if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60) {
                clock.fillByHMS(hour, minute, second);
                clock.setTime();
                serial.print(F("Time set to: "));
                serial.println(getFormattedDate(false, false, true));
            } else
                invalidValue = true;
            isConfigCommand = false;
        } else
            invalidSyntax = true;
        if (isConfigCommand && !invalidSyntax && !invalidValue) {
            EEPROM.update(0, 135);
            EEPROM.put(1, config);
            logConfig();
        } else if (invalidSyntax)
            serial.println(F("Invalid command syntax! Check the documentation"));
        else if (invalidValue)
            serial.println(F("Invalid value! Check the documentation"));
    }
}

void maintainMode() {
}