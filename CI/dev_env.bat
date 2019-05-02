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
  set VCVER=14.0
) else (
  set VCVER=%4
)

set TARGET=%SHORT_TARGET%
if "%TARGET%"=="10" (
  set TARGET=10.0.17763.0
)

:: Setup home dir (so that ssh configuration works fine)
if "%USERNAME%"=="otbbot" (
  set HOMEDRIVE=C:
  set HOMEPATH=\Users\otbbot
)
echo Home dir: %HOMEDRIVE%%HOMEPATH%

:: Setup Python
set PATH=%PATH%;C:\tools\Python35-%ARCH%
set PATH=%PATH%;C:\tools\Python35-%ARCH%\Scripts

:: Setup compiler
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" %ARCH% %TARGET% -vcvars_ver=%VCVER%

:: Setup Clcache
set CLCACHE_DIR=C:\clcache\%PROJECT%-%ARCH%-%TARGET%-%VCVER%
set CLCACHE_HARDLINK=1
:: set CLCACHE_SERVER=1
set CLCACHE_CL=
for /F "delims=" %%a in ('where cl.exe') do @if defined CLCACHE_CL (break ) else (set CLCACHE_CL=%%a)

echo CL path: "%CLCACHE_CL%"

:: install clcache.exe as cl.exe
copy C:\tools\Python35-%ARCH%\Scripts\clcache.exe C:\clcache\cl.exe
set PATH=C:\clcache;%PATH%

:: we need to change cache max size: clcache -M <size-in-bytes>
if "%PROJECT%"=="xdk" (
  call "clcache.exe" -M 3000000000
)
if "%PROJECT%"=="otb" (
  call "clcache.exe" -M 2000000000
)

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
