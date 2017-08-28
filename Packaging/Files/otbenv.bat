::
:: Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
::
:: This file is part of Orfeo Toolbox
::
::   https://www.orfeo-toolbox.org/
::
:: Licensed under the Apache License, Version 2.0 (the "License");
:: you may not use this file except in compliance with the License.
:: You may obtain a copy of the License at
::
::   http://www.apache.org/licenses/LICENSE-2.0
::
:: Unless required by applicable law or agreed to in writing, software
:: distributed under the License is distributed on an "AS IS" BASIS,
:: WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
:: See the License for the specific language governing permissions and
:: limitations under the License.

:: Setup environment for OTB package
set CURRENT_SCRIPT_DIR=%~dp0

set PATH=%CURRENT_SCRIPT_DIR%bin;%PATH%
set GDAL_DATA=%CURRENT_SCRIPT_DIR%share\data
set GEOTIFF_CSV=%CURRENT_SCRIPT_DIR%share\epsg_csv
set PYTHONPATH=%CURRENT_SCRIPT_DIR%lib\python;%PYTHONPATH%
set OTB_APPLICATION_PATH=%CURRENT_SCRIPT_DIR%lib\otb\applications
set GDAL_DRIVER_PATH=disable
:: Set numeric locale to C
set LC_NUMERIC=C
