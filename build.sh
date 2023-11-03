#!/bin/bash

#####                   Constants                      #####

RED="\033[31m"
YELLOW="\033[33m"
WHITE="\033[37m"
RESET="\033[0m"

CURRENT_DATE="$(date +'%d-%m-%Y')"
LOG_NUM=0

while [[ -f "${CURRENT_DATE}_$LOG_NUM.log" ]]; do
  ((LOG_NUM++))
done

LOG_FILE="${CURRENT_DATE}_$LOG_NUM.log"

#####                   Functions                      #####

function formattedPrint {
  toPrint="$(date +'%H-%M-%S') - $1"
  echo -e "$2$toPrint${RESET}"
  if [ "$log_to_file" = "true" ]; then
    echo "$toPrint" >> "$LOG_FILE"
  fi
}

function log {
  formattedPrint "Log: $1" "$WHITE"
}

function warn {
  formattedPrint "Warn: $1" "$YELLOW"
}

function error {
  formattedPrint "Error: $1" "$RED"
}

#####                   Main                      #####

log "####################"
log "#   3W build tool  #"
log "####################"

if [ $# -eq 1 ]; then
    if [[ "$1" = "-c="* ]]; then
        config_file=$(echo "$1" | awk -F= '{print $2}')
    else
        error "Bad usage, correct usage: -c=config_file"
        exit 1
    fi
else
    config_file="build.conf"
    log "No config file specified. Using default \"$config_file\""
fi

if [ -f "$config_file" ]; then
    log "\"$config_file\" file found. Loading values"
    # shellcheck disable=SC1090
    source $config_file
    log_to_file=$(echo "$log_to_file" | tr -d '\r\n')
    config_constants_file=$(echo "$config_constants_file" | tr -d '\r\n')
    eeprom_flag=$(echo "$eeprom_flag" | tr -d '\r\n')
    version=$(echo "$version" | tr -d '\r\n')
    mode=$(echo "$mode" | tr -d '\r\n')

    if [ "$log_to_file" != "true" ] && [ "$log_to_file" != "false" ]; then
        error "Log to file \"$log_to_file\" is not valid, it must be \"true\" or \"false\""
        exit 1
    fi
    if [ ! -f "$config_constants_file" ]; then
        error "Folder \"$config_constants_file\" file doesn't exist"
        exit 1
    fi
    if [[ ! "$eeprom_flag" =~ ^[0-9]+$ ]] || [ "$eeprom_flag" -lt 0 ] || [ "$eeprom_flag" -gt 255 ]; then
        error "EEPROM flag \"$eeprom_flag\" is not valid, it must be a number between 0 and 255"
        exit 1
    fi
    if [[ ! "$version" =~ ^[0-9.]+$ ]]; then
        error "Version \"$version\" is not valid, it must be composed only of numbers and dots"
        exit 1
    fi
    if [ "$mode" != "compile" ] && [ "$mode" != "upload" ] && [ "$mode" != "monitor" ]; then
        error "Mode \"$mode\" is not valid, it must be \"compile\", \"upload\" or \"monitor\""
        exit 1
    fi

    log "Loaded constants are valid"
else
    warn "Config file \"$config_file\" not found. Using default values"
fi

if [ -z "$log_to_file" ]; then
    log_to_file="false"
fi
if [ -z "$config_constants_file" ]; then
    config_constants_file="src/h/data.h"
fi
if [ -z "$eeprom_flag" ]; then
    eeprom_flag="0"
fi
if [ -z "$version" ]; then
    version="0.0"
fi
if [ -z "$mode" ]; then
    mode="compile"
fi

log "######### Config ###########"
log "Log to file: $log_to_file"
log "Constants file: $config_constants_file"
log "EEPROM flag: $eeprom_flag"
log "Version: $version"
log "Mode: $mode"
log "########## End #############"

sed -i "s/^#define EEPROM_FLAG .*/#define EEPROM_FLAG $eeprom_flag/" "$config_constants_file"
if grep -q "^#define EEPROM_FLAG $eeprom_flag" "$config_constants_file"; then
    log "EEPROM flag has been updated to \"$eeprom_flag\" in \"$config_constants_file\""
else
    error "Failed to update EEPROM flag to \"$eeprom_flag\" in \"$config_constants_file\""
    exit 1
fi

sed -i "s/^#define VERSION F(\".*\")/#define VERSION F(\"$version\")/" "$config_constants_file"
if grep -q "^#define VERSION F(\"$version\")" "$config_constants_file"; then
    log "Version has been updated to \"$version\" in \"$config_constants_file\""
else
    error "Failed to update version to \"$version\" in \"$config_constants_file\""
    exit 1
fi

lot_id=$(openssl rand -hex 10 | cut -c1-20)

sed -i "s/^#define LOT_ID F(\".*\")/#define LOT_ID F(\"$lot_id\")/" "$config_constants_file"
if grep -q "^#define LOT_ID F(\"$lot_id\")" "$config_constants_file"; then
    log "Lot ID has been updated to \"$lot_id\" in \"$config_constants_file\""
else
    error "Failed to update lot ID to \"$lot_id\" in \"$config_constants_file\""
    exit 1
fi

if ! command -v pio > /dev/null 2>&1; then
  error "PlatformIO is not installed on your system"
  error "Please install it from https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html"
  exit 1
fi

log "Starting to build the project in \"$mode\" mode"
if [ "$mode" = "compile" ]; then
    pio run
elif [ "$mode" = "upload" ]; then
    pio run -t upload
elif [ "$mode" = "monitor" ]; then
    pio pio run -t upload -t monitor
fi

read -p -r "Press enter to exit"
