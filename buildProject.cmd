@echo off
set BuidToolsRoot=C:/Tools
set QT64_LOCATION=%BuidToolsRoot%/Qt5.15.2/5.15.2/msvc2019_64
set Poco_DIR=%BuidToolsRoot%/Poco/

call :SetCMakeConfig %sCMakeConfig%

IF "%1" == "-h" (
    echo Options
    echo    "-h"          = Help
    echo    "-p"          = Only generate project
    echo    "-c"          = Clear previous build results. Rebuld all. Optional
    echo    Selected configuration or all configurations, if not specified
    echo        "-r"      = Release
    echo        "-d"      = Debug
    echo        "-rwdi"   = Release With Debug Info
    echo    "-i"          = Install
    goto:EOF
)

echo flags instance "%~1"

set /a rebuild  = 0
set /a release = 0
set /a debug = 0
set /a releasedebug = 0
set /a installresults = 0
set /a onlygenerateproject = 0
set /a nRetStatus=0

for %%i in (%*) do (
  IF "%%i" == "-r" set /a release = 1
  IF "%%i" == "-d" set /a debug = 1
  IF "%%i" == "-rwdi" set /a releasedebug = 1
  IF "%%i" == "-c" set /a rebuild = 1
  IF "%%i" == "-i" set /a installresults = 1
  IF "%%i" == "-p" set /a onlygenerateproject = 1
)

set config_types=Debug;Release;RelWithDebInfo
set install_types=
if !debug! == 1 (
    if "%install_types%" neq "" set install_types=%install_types%";"
    set install_types=%install_types%"Debug"
)
if !release! == 1 (
    if "%install_types%" neq "" set install_types=%install_types%";"
    set install_types=%install_types%"Release"
)
if !releasedebug! == 1 (
    if "%install_types%" neq "" set install_types=%install_types%";"
    set install_types=%install_types%"RelWithDebInfo"
)
if !debug! == 0 if !release! == 0 if !releasedebug! == 0 set install_types=%config_types%
set install_types=%config_types%

SET build_path="%cd%/builds"
SET install_path="%cd%/install"

IF %onlygenerateproject% == 1 (
    echo Generate project
    cmake -S"." -G%sCMakeConfig% -B%build_path% -A"x64" -DCMAKE_INSTALL_PREFIX=%install_path% -DCMAKE_CONFIGURATION_TYPES="%config_types%" -DCMAKE_INSTALL_CONFIG_NAME="%install_types%" -DCMAKE_INSTALL_MESSAGE=LAZY
    goto:EOF
)

IF %rebuild% == 1 (
    IF EXIST %build_path% (
        echo Clear previous build results. Rebuld all
        rd /S /Q %build_path%
    )
)

goto :Return

:SetCMakeConfig
rem echo CMake %1
echo АЦУАПЦУ4АПУЦ
if "%~1" == "" (
    echo setting sCMakeConfig
    set sCMakeConfig="Visual Studio 15 2017"
    set sInstallVS=vs17
) else (
    echo sCMakeConfig exist
    if "%~1" == "Visual Studio 15 2017" (
        set sInstallVS=vs17
    ) else (
		if "%~1" == "Visual Studio 16 2019" (
			set sInstallVS=vs19
		) else (
			set sInstallVS=vs22	
		)
    )
)
echo %sCMakeConfig% %sInstallVS%
exit /b

:Return
@rem echo exit /b !nRetStatus!
exit /b !nRetStatus!