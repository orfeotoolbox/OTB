:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Monteverdi launcher to set up proper GDAL environment
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:: Get the directory of the current script 
@set CURRENT_SCRIPT_DIR=%~dp0

:: Set GDAL_DATA env. variable
@set GDAL_DATA=%CURRENT_SCRIPT_DIR%..\share\gdal
@set ITK_AUTOLOAD_PATH=%CURRENT_SCRIPT_DIR%..\lib\otb\applications

:: Set current dir to HOME dir because Monteverdi generates temporary files and need write access
@cd %HOMEDRIVE%%HOMEPATH%

:: Start Monteverdi
@start "Monteverdi" /B "%CURRENT_SCRIPT_DIR%monteverdi2.exe" %*
