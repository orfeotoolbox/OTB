:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Monteverdi launcher to set up proper environment
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:: Set locale numeric to C
@set LC_NUMERIC=C

:: Get the directory of the current script
@set CURRENT_SCRIPT_DIR=%~dp0

:: Set GDAL_DATA env. variable
@set GDAL_DATA=%CURRENT_SCRIPT_DIR%share\gdal

:: Set GEOTIFF_CSV env. variable
@set GEOTIFF_CSV=%CURRENT_SCRIPT_DIR%share\epsg_csv

:: Performance issue: do not set ITK_AUTOLOAD_PATH to avoid loading of OTB-Applications.
@set OTB_APPLICATION_PATH=%CURRENT_SCRIPT_DIR%lib\otb\applications

:: Set current dir to HOME dir because Monteverdi generates temporary files and need write access
@cd %HOMEDRIVE%%HOMEPATH%

:: Start Monteverdi
@start "Monteverdi Viewer" /B "%CURRENT_SCRIPT_DIR%bin\monteverdi.exe" %*
