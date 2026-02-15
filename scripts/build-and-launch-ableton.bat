@echo off

REM Ableton cannot load the updated plugin after it has been re-built and
REM I was tired of manually opening and closing it every time I made a change,
REM so this script automates that process.

if "%~1"=="-h" goto :usage
if "%~1"=="--help" goto :usage

ECHO ############################################################
ECHO Closing Ableton Live 12 Suite...
ECHO ############################################################
REM Close Ableton Live 12 Suite if running
TASKKILL /IM "Ableton Live 12 Suite.exe" /F >nul 2>&1

ECHO ############################################################
ECHO Building project...
ECHO ############################################################
cmake -B build -S . -G "Visual Studio 18 2026"
cmake --build build --config Debug

ECHO ############################################################
ECHO Launching Ableton Live 12 Suite with project %1
ECHO ############################################################
START "" "C:\ProgramData\Ableton\Live 12 Suite\Program\Ableton Live 12 Suite.exe" %1
GOTO :EOF

:usage
ECHO.
ECHO Builds the project and restarts Ableton Live with the specified project.
ECHO.
ECHO Usage:
ECHO   build-and-launch-ableton.bat path\to\project.als
EXIT /B 0
