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
:: check input arguments
if %1.==. (
  echo "No arch"
  call :Help
  goto :eof
)

if %2.==. (
  echo "No project"
  call :Help
  goto :eof
)

if /I "%1"=="help" (
  call :Help
  goto :eof
)

if /I "%1"=="/help" (
  call :Help
  goto :eof
)

if /I "%1"=="-help" (
  call :Help
  goto :eof
)

set ARCH=%1
set PROJECT=%2

if %3.==. (
  set SHORT_TARGET=10
) else (
  set SHORT_TARGET=%3
)

if %4.==. (
  set VCVER=14.29
) else (
  set VCVER=%4
)

set TARGET=%SHORT_TARGET%
if "%TARGET%"=="10" (
  set TARGET=10.0.17763.0
)

:: Setup home dir (so that ssh configuration works fine)
:: if "%USERNAME%"=="otbbot" (
  set HOMEDRIVE=C:
  set HOMEPATH=\Users\otbbot
::)
echo Home dir: %HOMEDRIVE%%HOMEPATH%

:: Setup Python
set PATH=C:\tools\Python37-%ARCH%;%PATH%
set PATH=C:\tools\Python37-%ARCH%\Scripts;%PATH%

:: Setup GL dlls
set PATH=%PATH%;C:\tools\GL\%ARCH%\bin

:: Setup compiler
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" %ARCH% %TARGET% -vcvars_ver=%VCVER%

:: Setup Clcache
set BUILDCACHE_DIR=C:\buildcache\%PROJECT%-%ARCH%-%TARGET%-%VCVER%

if "%PROJECT%"=="xdk" (
  set BUILDCACHE_MAX_CACHE_SIZE=3000000000
)
if "%PROJECT%"=="otb" (
  set BUILDCACHE_MAX_CACHE_SIZE=2000000000
)
set PATH=C:\tools\buildcache\bin;%PATH%

set IMAGE_NAME=windows-%SHORT_TARGET%-%ARCH%-vc%VCVER%
echo Generated IMAGE_NAME: %IMAGE_NAME%

:: setup path to perl, but add it last ... (there is a libstdc++.dll in that folder...)
set PATH=%PATH%;C:\tools\perl\perl\bin

goto :eof

:Help
setlocal
echo "Usage: dev_env.bat <compiler_arch>  <project>  [<target-os>  [<vc_version>]]"
echo "  <compiler_arch> : 'x86' | 'x64'"
echo "  <project>       : 'xdk' | 'otb'"
echo "  <target-os>     : '8.1' | '10' (default)"
echo "  <vc_version>    :"
echo "    '14.20' (i.e. VS 2019)"
echo "    '14.16' (i.e. VS 2017)"
echo "    '14.0'  (i.e. VS 2015) (default)"
endlocal
