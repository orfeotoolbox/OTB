:: 
:: Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
::
:: This file is part of Orfeo Toolbox
:: 
::    https://www.orfeo-toolbox.org/
::
:: Licensed under the Apache License, Version 2.0 (the "License");
:: you may not use this file except in compliance with the License.
:: You may obtain a copy of the License at
::
::    http://www.apache.org/licenses/LICENSE-2.0
::
:: Unless required by applicable law or agreed to in writing, software
:: distributed under the License is distributed on an "AS IS" BASIS,
:: WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
:: See the License for the specific language governing permissions and
:: limitations under the License.

:: script to uninstall OTB

setlocal
set CUR_DIR=%~dp0
cd %CUR_DIR%..
set MY_INSTALL_DIR=%cd%
del /S /Q %MY_INSTALL_DIR%\include\OTB* || exit 1
del /S /Q %MY_INSTALL_DIR%\lib\cmake\OTB* || exit 1
del /S /Q %MY_INSTALL_DIR%\lib\otb* || exit 1
del /S /Q %MY_INSTALL_DIR%\lib\python\_otbApplication.* || exit 1
del /S /Q %MY_INSTALL_DIR%\bin\otb* || exit 1
del /S /Q %MY_INSTALL_DIR%\bin\monteverdi.exe || exit 1
del /S /Q %MY_INSTALL_DIR%\bin\mapla.exe || exit 1
del /S /Q %MY_INSTALL_DIR%\mapla.bat || exit 1
del /S /Q %MY_INSTALL_DIR%\monteverdi.bat || exit 1
del /S /Q %MY_INSTALL_DIR%\share\OTB* || exit 1
del /S /Q "%MY_INSTALL_DIR%\OTB Project.zip" || exit 1
endlocal


