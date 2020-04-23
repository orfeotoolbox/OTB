::
:: Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
::
:: This file is part of Orfeo Toolbox
::
::     https://www.orfeo-toolbox.org/
::
:: Licensed under the Apache License, Version 2.0 (the "License");
:: you may not use this file except in compliance with the License.
:: You may obtain a copy of the License at
::
::     http://www.apache.org/licenses/LICENSE-2.0
::
:: Unless required by applicable law or agreed to in writing, software
:: distributed under the License is distributed on an "AS IS" BASIS,
:: WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
:: See the License for the specific language governing permissions and
:: limitations under the License.

@echo off

:: script to uninstall OTB

setlocal
cd %~dp0%..

echo - Clean include\OTB*
for /f %%i in ('dir /b include\OTB*') do rd /S /Q include\%%i

echo - Clean lib\cmake\OTB*
for /f %%i in ('dir /b lib\cmake\OTB*') do rd /S /Q lib\cmake\%%i

echo - Clean share\otb
rd /S /Q share\otb

echo - Clean lib\otb
rd /S /Q lib\otb

del /S /Q lib\otb*
del /S /Q lib\python\*otbApplication.* || exit 1
del /S /Q bin\otb* || exit 1
del /S /Q bin\monteverdi.exe || exit 1
del /S /Q bin\mapla.exe || exit 1
del /S /Q mapla.bat || exit 1
del /S /Q monteverdi.bat || exit 1
del /S /Q otbenv.* || exit 1
del /S /Q start_devenv.bat || exit 1
del /S /Q "OTB Project.zip" || exit 1

echo OTB is now uninstalled from %cd%

endlocal
