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

rem set EXIT_ON_ERROR=0
rem set TEST_VERBOSE=1
rem cmd /k tools\selftester.bat /q > selftest_report.log 2>&1

set MY_TOOLS_DIR=%~dp0
cd %MY_TOOLS_DIR%..
set MY_INSTALL_DIR=%cd%
echo "MY_INSTALL_DIR=%MY_INSTALL_DIR%"

:: this is set in wrapper script
::set EXIT_ON_ERROR=0
IF %EXIT_ON_ERROR%.==. ( set EXIT_ON_ERROR=1)
IF %TEST_VERBOSE%.==. ( set TEST_VERBOSE=1)

echo TEST_VERBOSE=%TEST_VERBOSE%
echo EXIT_ON_ERROR=%EXIT_ON_ERROR%

:: if "/q" is given, hide the error dialogs (hence make the script non-interactive)
if /i not -%1-==-/q- (
  goto :skip_hide_error
)


call :getDontShowUI
if %DONTSHOWUI_VALUE% equ 0 call :setDontShowUI 1
:skip_hide_error
echo Hide error dialogs
:: Get current PID
type NUL > pid.log
wmic PROCESS where "Name='WMIC.exe'" get ParentProcessId >pid.log 2>&1
for /F "TOKENS=1" %%b in ('type pid.log ^| findstr [0-9]') do set CURRENT_PID=%%b

:: setup the otbenv
call otbenv.bat

:: -----------------------------------------------------------------------------------
REM Check 1 : check binaries
for /R %%i in (*.dll) do (
  call :check_binary %%i
)
for %%i in (bin\mapla.exe,bin\monteverdi.exe,bin\otbApplicationLauncherCommandLine.exe,bin\otbApplicationLauncherQt.exe) do (
  call :check_binary %%i
)
call :check_binary lib\python\_otbApplication.pyd

:: -----------------------------------------------------------------------------------
rem Check 2 : check applications and Python wrapping
set /a app_count=0
for /R %%i in (lib\otb\applications\otbapp_*.dll) do (
  call :check_application %%i
)
if %app_count% leq 90 (
  echo WARNING: only %app_count% applications found ! Expected at least 90
)
call :check_python_wrapping

:: -----------------------------------------------------------------------------------
rem Check 3 : check monteverdi & mapla
call :check_desktop_app monteverdi
call :check_desktop_app mapla 20

if /i not -%1-==-/q- (
  goto :skip_restore_error
)
if %DONTSHOWUI_VALUE% equ 0 call :setDontShowUI 0
:skip_restore_error
setlocal
del pid.log
del tmp.log
endlocal
goto :eof

::########################[ FUNCTIONS ]##################################
:check_binary
setlocal
if %TEST_VERBOSE% gtr 1 ( echo ":check_binary %1" )
type NUL > tmp.log
tools\otb_loader.exe %1 > tmp.log 2>&1
call :nb_tmp_lines
if %nb_tmp_lines_out% gtr 0 (
  echo "ERROR: otb_loader.exe %1"
  type tmp.log
  if %EXIT_ON_ERROR% equ 1 ( exit 1 )
)
endlocal
goto :eof
::-----------------------------------------------------------------------
:check_application
setlocal
set dll_path=%~n1
set app=%dll_path:~7%
if %TEST_VERBOSE% equ 1 ( echo ":check_application %app%" )
if not exist bin\otbcli_%app%.bat (
  echo ERROR : missing cli launcher for application %app%
  if %EXIT_ON_ERROR% equ 1 ( exit 1 )
  goto :check_gui
)
type NUL > tmp.log
cmd /C bin\otbcli_%app%.bat "-help" ^> tmp.log ^2^>^&^1
call :parse_cli_output
if %parse_cli_result% equ 0 (
  echo ERROR : error launching application otbcli_%app%
  type results.txt 
  if %EXIT_ON_ERROR% equ 1 ( exit 1 )
  )

:check_gui
if not exist bin\otbgui_%app%.bat (
  echo ERROR : missing gui launcher for application %app%
  if %EXIT_ON_ERROR% equ 1 ( exit 1 )
  goto :check_application_end
)
if %app_count% geq 2 goto :check_application_end
type NUL > tmp.log
start "otbgui application" /b bin\otbgui_%app%.bat ^> tmp.log ^2^>^&^1
timeout 5 >nul
call :get_child_pid %CURRENT_PID% cmd.exe
set first_child=0
if %child_pid% gtr 1 (
  set first_child=%child_pid%
) else (
  echo ERROR : could not launch otbgui_%app%
  type tmp.log 
  goto :check_application_clean
)
call :get_child_pid %first_child% otbApplicationLauncherQt.exe
if not %child_pid% gtr 1 (
  echo ERROR : could not launch otbApplicationLauncherQt %app%
  type tmp.log
  if %EXIT_ON_ERROR% equ 1 ( exit 1 )
)
  
:check_application_clean
if not %first_child% == 0 (
  taskkill /PID %first_child% /F /T > NUL 2>&1
)
  
:check_application_end
endlocal & set /a app_count=app_count+1
goto :eof
::-----------------------------------------------------------------------
:check_desktop_app
setlocal
set appName=%1
set delay=5
if not -%2-==-- set delay=%2
type NUL > tmp.log
start "Desktop app" /b bin\%appName%.exe > tmp.log 2>&1
timeout %delay% >nul
call :get_child_pid %CURRENT_PID% %appName%.exe
if %child_pid% gtr 1 (
  taskkill /PID %child_pid% /F /T > NUL 2>&1
) else (
  echo ERROR : could not launch %appName%.exe
  type tmp.log
  if %EXIT_ON_ERROR% equ 1 ( exit 1 )
)
endlocal
goto :eof
::-----------------------------------------------------------------------
:check_python_wrapping
setlocal
type NUL > tmp.log
python -c "import otbApplication" > tmp.log 2>&1
call :nb_tmp_lines
if %nb_tmp_lines_out% gtr 0 (
  echo ERROR : failed to run python wrapping
  type tmp.log
  if %EXIT_ON_ERROR% equ 1 ( exit 1 )
)
endlocal
goto :eof
::-----------------------------------------------------------------------
:parse_cli_output
setlocal
set /a ret=1
findstr /n /r /c:"^This is the %app% application, version " tmp.log  > results.txt
if %errorlevel%==1 (
  set /a ret=0
  echo "findstr failed 1st regex in parse_cli_output"
  goto :parse_cli_output_end  
)
findstr /n /r /c:"^Parameters: " tmp.log  > results.txt
if %errorlevel%==1 (
  set /a ret=0
  echo "findstr failed 2nd regex in parse_cli_output"
  goto :parse_cli_output_end  
)
findstr /n /r /c:"^Examples: " tmp.log  > results.txt
if %errorlevel%==1 (
  set /a ret=0
    echo "findstr failed 3rd regex in parse_cli_output"
  goto :parse_cli_output_end  
)
findstr /n /r /c:"FATAL" tmp.log  > results.txt
if %errorlevel%==0 (
  set /a ret=0
    echo "findstr passed 4th regex in parse_cli_output"
  goto :parse_cli_output_end  
)

:parse_cli_output_end
endlocal & set /a parse_cli_result=%ret%
goto :eof
::-----------------------------------------------------------------------

:nb_tmp_lines
setlocal
for /F "delims=" %%i in ('find /C /V "" ^< tmp.log') do set output=%%i
endlocal & set nb_tmp_lines_out=%output%
goto :eof
::-----------------------------------------------------------------------
:get_child_pid
setlocal
set parent_pid=%1
set process_name=%2
set output=0
type NUL > pid.log
wmic PROCESS where (ParentProcessId=%parent_pid% AND Name="%process_name%") get ProcessId >pid.log 2>&1
for /F "TOKENS=1" %%a in ('type pid.log ^| findstr [0-9]') do set output=%%a
endlocal & set child_pid=%output%
goto :eof
::-----------------------------------------------------------------------
:getDontShowUI
setlocal
set output=0
for /F "TOKENS=3" %%a in ('reg query "HKCU\Software\Microsoft\Windows\Windows Error Reporting" /v DontShowUI ^| findstr DontShowUI') do set output=%%a
endlocal & set DONTSHOWUI_VALUE=%output:~2%
goto :eof
::-----------------------------------------------------------------------
:setDontShowUI
setlocal
reg add "HKCU\Software\Microsoft\Windows\Windows Error Reporting" /v DontShowUI /t REG_DWORD /d %1 /f
endlocal
goto :eof

