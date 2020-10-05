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
::

@echo on

set CURRENT_SCRIPT_DIR=%~dp0
set CURRENT_SCRIPT_DIR=%CURRENT_SCRIPT_DIR:~0,-1%


set PATH=%CURRENT_SCRIPT_DIR%\bin;%PATH%
set VSDEV_ENV=C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.exe
set OTB_VS_TEMPLATE_DIR=%USERPROFILE%\Documents\Visual Studio 2015\Templates\ProjectTemplates

set OTB_XDK_DIR=%CURRENT_SCRIPT_DIR:\=/%
::dir %OTB_VS_TEMPLATE_DIR%
if exist "%VSDEV_ENV%" (
if not exist "%OTB_VS_TEMPLATE_DIR%" (   md "%OTB_VS_TEMPLATE_DIR%" )
copy "%CURRENT_SCRIPT_DIR%\OTB Project.zip" "%OTB_VS_TEMPLATE_DIR%"
START "" "%VSDEV_ENV%"
) else (
echo "%VSDEV_ENV% not found"
)

@cmd
