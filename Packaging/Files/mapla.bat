:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Mapla launcher to set up proper environment
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
@echo off
setlocal

:: Get the directory of the current script
set CURRENT_SCRIPT_DIR=%~dp0

:: Setup environment
call %CURRENT_SCRIPT_DIR%otbenv.bat

:: Set current dir to HOME dir because Monteverdi generates temporary files and need write access
cd %HOMEDRIVE%%HOMEPATH%

:: Start Monteverdi
start "Monteverdi Application Launcher" /B "%CURRENT_SCRIPT_DIR%bin\mapla.exe" %*

endlocal
