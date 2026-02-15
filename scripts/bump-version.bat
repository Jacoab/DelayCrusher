@echo off
REM bump-version.bat: Bump semantic version in CMakeLists.txt and GitHub workflow files, preserving whitespace

setlocal enabledelayedexpansion

if "%~1"=="" (
    echo Error: Missing argument
    echo Use: bump-version.bat ^<major^|minor^|patch^>
    exit /b 1
)

if "%~1"=="-h" goto :usage
if "%~1"=="--help" goto :usage

if "%~1"=="major" (
    set "BUMP_TYPE=major"
) else if "%~1"=="minor" (
    set "BUMP_TYPE=minor"
) else if "%~1"=="patch" (
    set "BUMP_TYPE=patch"
) else (
    echo Error: Invalid argument '%~1'
    goto :usage
)

REM Extract current version from CMakeLists.txt
set "CURRENT_VERSION="
for /f "usebackq delims=" %%v in (`powershell -NoProfile -Command "(Select-String -Path 'CMakeLists.txt' -Pattern 'set\(PROJECT_VERSION' | Select-Object -First 1).Line -replace '.*?(\d+\.\d+\.\d+).*','$1'"`) do set "CURRENT_VERSION=%%v"

if "%CURRENT_VERSION%"=="" (
    echo Error: Could not find PROJECT_VERSION in CMakeLists.txt
    exit /b 1
)

REM Split version into components
for /f "tokens=1,2,3 delims=." %%a in ("%CURRENT_VERSION%") do (
    set "MAJOR=%%a"
    set "MINOR=%%b"
    set "PATCH=%%c"
)

REM Increment appropriate component
if "%BUMP_TYPE%"=="major" (
    set /A MAJOR+=1
    set "MINOR=0"
    set "PATCH=0"
) else if "%BUMP_TYPE%"=="minor" (
    set /A MINOR+=1
    set "PATCH=0"
) else (
    set /A PATCH+=1
)

set "NEW_VERSION=%MAJOR%.%MINOR%.%PATCH%"

echo Bumping version: %CURRENT_VERSION% -^> %NEW_VERSION%

REM Only replace the numeric version, preserving whitespace
powershell -NoProfile -Command "$new='%NEW_VERSION%'; (Get-Content 'CMakeLists.txt') -replace '(?<=set\(PROJECT_VERSION\s*)\d+\.\d+\.\d+(?=\))', $new | Set-Content 'CMakeLists.txt'"
echo [OK] Updated CMakeLists.txt

if exist ".github\workflows\branch-build.yml" (
    powershell -NoProfile -Command "$new='%NEW_VERSION%'; (Get-Content '.github\workflows\branch-build.yml') -replace '(?<=PROJECT_VERSION:\s*)\d+\.\d+\.\d+', $new | Set-Content '.github\workflows\branch-build.yml'"
    echo [OK] Updated .github\workflows\branch-build.yml
)

if exist ".github\workflows\release.yml" (
    powershell -NoProfile -Command "$new='%NEW_VERSION%'; (Get-Content '.github\workflows\release.yml') -replace '(?<=PROJECT_VERSION:\s*)\d+\.\d+\.\d+', $new | Set-Content '.github\workflows\release.yml'"
    echo [OK] Updated .github\workflows\release.yml
)

echo Version bump complete: %NEW_VERSION%
endlocal

exit /b 0

:usage
echo.
echo Bumps the project version.
echo.
echo Usage:
echo   bump-version.bat ^<major^|minor^|patch^>
echo.
echo Examples:
echo   bump-version.bat major  REM 1.0.0 -^> 2.0.0
echo   bump-version.bat minor  REM 1.0.0 -^> 1.1.0
echo   bump-version.bat patch  REM 1.0.0 -^> 1.0.1