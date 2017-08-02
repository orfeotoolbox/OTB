:: Setup environment for OTB package
set CURRENT_SCRIPT_DIR=%~dp0

set PATH=%CURRENT_SCRIPT_DIR%bin;%PATH%
set GDAL_DATA=%CURRENT_SCRIPT_DIR%share\data
set GEOTIFF_CSV=%CURRENT_SCRIPT_DIR%share\epsg_csv
set PYTHONPATH=%CURRENT_SCRIPT_DIR%lib\python;%PYTHONPATH%
set OTB_APPLICATION_PATH=%CURRENT_SCRIPT_DIR%lib\otb\applications

:: Set numeric locale to C
set LC_NUMERIC=C
