#!/bin/bash
set -e

if [ "$1" != "build" ] && [ -f "$1" ]; then
  export IDF_PATH=$1
  shift
fi
if [ -z "$IDF_PATH" ]; then
  export IDF_PATH=$HOME/esp/esp-idf
fi
if [ ! -f "$IDF_PATH/export.sh" ]; then
  echo "ERROR: ESP-IDF at '$IDF_PATH' does not exist or is corrupted"
  exit 1
fi

if [ -z "$IDF_PYTHON_ENV_PATH" ]; then
  source $IDF_PATH/export.sh
fi

idf.py $@

if [ -f "$IDF_PATH/tools/gen-flash-cmds.sh" ]; then
  $IDF_PATH/tools/gen-flash-cmds.sh
fi
