RED="\033[31m"
YELLOW="\033[33m"
LIGHT_BLUE="\033[34m"
WHITE="\033[37m"
GREEN="\033[32m"
RESET="\033[0m"

function log {
  echo -e "${WHITE}Log: $1${RESET}"
}

function warn {
  echo -e "${YELLOW}Warning: $1${RESET}"
}

function error {
  echo -e "${RED}Error: $1${RESET}"
}

function debug {
  echo -e "${LIGHT_BLUE}Debug: $1${RESET}"
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
    config_constants_file=$(echo "$config_constants_file" | tr -d '\r\n')
    version=$(echo "$version" | tr -d '\r\n')
    mode=$(echo "$mode" | tr -d '\r\n')

    if [ ! -f "$config_constants_file" ]; then
        error "Folder \"$config_constants_file\" file doesn't exist"
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

if [ -z "$config_constants_file" ]; then
    config_constants_file="src/h/data.h"
fi
if [ -z "$version" ]; then
    version="0.0"
fi
if [ -z "$mode" ]; then
    mode="compile"
fi

log "######### Config ###########"
log "Constants file: $config_constants_file"
log "Version: $version"
log "Mode: $mode"
log "########## End #############"

