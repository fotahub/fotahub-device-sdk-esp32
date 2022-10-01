@echo off

if not "%1" == "build" (
  if exist "%1" (
    set IDF_PATH=%1
    goto removeFirstArg
  )
)
goto :readSecondArg
:removeFirstArg
shift

:readSecondArg
if not "%1" == "build" (
  if exist "%1" (
    set WHO_PATH=%1
    goto removeSecondArg
  )
)
goto validateEnv
:removeSecondArg
shift

:validateEnv
if "%IDF_PATH%" == "" (
  set IDF_PATH=%USERPROFILE%\esp\esp-idf
)
if not exist "%IDF_PATH%\export.bat" (
  echo ERROR: ESP-IDF at '%IDF_PATH%' does not exist or is corrupted
  exit /B 1
)
if "%WHO_PATH%" == "" (
  set WHO_PATH=%USERPROFILE%\esp\esp-who
)
if not exist "%WHO_PATH%\components" (
  echo ERROR: ESP-WHO at '%WHO_PATH%' does not exist or is corrupted
  exit /B 1
)

if "%IDF_PYTHON_ENV_PATH%" == "" (
  set "Path=%USERPROFILE%\AppData\Local\Microsoft\WindowsApps;%Path%"
  call %IDF_PATH%\export.bat
  if errorlevel 1 exit /B 1
)

idf.py %1 %2 %3 %4 %5 %6 %7 %8 %9
