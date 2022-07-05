@echo off

if not "%1" == "build" (
  if exist "%1" (
    set IDF_PATH=%1
    goto removeFirstArg
  )
)
goto validateEnv
:removeFirstArg
shift
:validateEnv
if "%IDF_PATH%" == "" (
  set IDF_PATH=%USERPROFILE%\esp\esp-idf
)
if not exist "%IDF_PATH%\export.bat" (
  echo ERROR: ESP-IDF at '%IDF_PATH%' does not exist or is corrupted
  exit /B 1
)

if "%IDF_PYTHON_ENV_PATH%" == "" (
  set "Path=%USERPROFILE%\AppData\Local\Microsoft\WindowsApps;%Path%"
  call %IDF_PATH%\export.bat
  if errorlevel 1 exit /B 1
)

if "%1" == "flash" goto flash 
if "%1" == "erase-flash" goto flash
goto default

:flash
set IDF_OPTIONS=-p NONE -b 921600
idf.py %IDF_OPTIONS% %1 %2 %3 %4 %5 %6 %7 %8 %9
exit /B 0

:monitor
set IDF_OPTIONS=-p NONE
idf.py %IDF_OPTIONS% %1 %2 %3 %4 %5 %6 %7 %8 %9
exit /B 0

:default
idf.py %1 %2 %3 %4 %5 %6 %7 %8 %9
if errorlevel 1 exit /B 1

if "%1" == "-B" goto yield
if "%1" == "--build-dir" goto yield
exit /B 0

:yield
if exist "%2\demoproduct.bin" (
    if not exist build md build
    copy "%2\demoproduct.bin" build
)
