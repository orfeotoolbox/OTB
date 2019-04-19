@echo off
:: check input arguments
if %1.==. (
  echo "No arch"
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

if %2.==. (
  set TARGET=10.0.17763.0
) else (
  set TARGET=%2
)

if %3.==. (
  set VCVER=14.0
) else (
  set VCVER=%3
)

:: Setup Python
set PATH=%PATH%;C:\tools\Python35-%ARCH%
set PATH=%PATH%;C:\tools\Python35-%ARCH%\Scripts

echo Error level %ERRORLEVEL%

:: Setup compiler
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" %ARCH% %TARGET% -vcvars_ver=%VCVER%

echo Error level %ERRORLEVEL%

:: Setup Clcache
set CLCACHE_DIR=C:\clcache\%ARCH%-%TARGET%-%VCVER%
set CLCACHE_HARDLINK=1
set CLCACHE_SERVER=1
set CLCACHE_CL=
for /F "delims=" %%a in ('where cl.exe') do @if defined CLCACHE_CL (break ) else (set CLCACHE_CL=%%a)

echo Error level %ERRORLEVEL%

echo CL path: "%CLCACHE_CL%"

:: if we need to change cache max size: clcache -M <size-in-bytes>

echo Error level %ERRORLEVEL%
goto :eof

:Help
setlocal
echo "Usage: dev_env.bat <compiler_arch>  [<target-os>  [<vc_version>]]"
echo "  <compiler_arch> : 'x86' | 'x64'"
echo "  <target-os>     : '8.1' | '10.0.17763.0' (default)"
echo "  <vc_version>    :"
echo "    '14.20' (i.e. VS 2019)"
echo "    '14.16' (i.e. VS 2017)"
echo "    '14.0'  (i.e. VS 2015) (default)"
endlocal
