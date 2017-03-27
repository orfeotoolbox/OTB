@echo off
setlocal

set CURRENT_SCRIPT_DIR=%~dp0
set DIRNAME=%CURRENT_SCRIPT_DIR%..
cd %DIRNAME%

:: Get current PID
type NUL > pid.log
wmic PROCESS where "Name='WMIC.exe'" get ParentProcessId >pid.log 2>&1
for /F "TOKENS=1" %%b in ('type pid.log ^| findstr [0-9]') do set CURRENT_PID=%%b

:: setup the otbenv
:: call otbenv.cmd

del selftest_report.log
type NUL > selftest_report.log
call :nb_report_lines
set REF_SIZE=%nb_report_lines_out%

rem Check 1 : check binaries
for /R %%i in (*.dll) do (
  call :check_binary %%i
)

for %%i in (bin\mapla.exe,bin\monteverdi.exe,bin\otbApplicationLauncherCommandLine.exe,bin\otbApplicationLauncherQt.exe) do (
  call :check_binary %%i
)

call :nb_report_lines
if %nb_report_lines_out% gtr %REF_SIZE% (
echo Check 1/3 : FAIL
rem should exit here
rem goto :eof
) else (
echo Check 1/3 : PASS
)
set REF_SIZE=%nb_report_lines_out%

rem Check 2 : check applications and Python wrapping
set /a app_count=0
for /R %%i in (lib\otb\applications\otbapp_*.dll) do (
  call :check_application %%i
)
if %app_count% leq 90 (
  echo WARNING: only %app_count% applications found ! Expected at least 90
)

call :nb_report_lines
if %nb_report_lines_out% gtr %REF_SIZE% (
echo Check 2/3 : FAIL
) else (
echo Check 2/3 : PASS
)
set REF_SIZE=%nb_report_lines_out%

rem Check 3 : check monteverdi & mapla


endlocal
goto :eof

rem functions :
:check_binary
setlocal
type NUL > tmp.log
bin\otb_loader.exe %1 > tmp.log 2>&1
call :nb_tmp_lines
if %nb_tmp_lines_out% gtr 0 (
  echo ERROR : otb_loader.exe %1
  echo otb_loader.exe %1 > selftest_report.log
  type tmp.log > selftest_report.log
)
endlocal
goto :eof

:check_application
setlocal
set dll_path=%~n1
set app=%dll_path:~7%
if not exist bin\otbcli_%app%.bat (
  echo ERROR : missing cli launcher for application %app%
  echo ERROR : missing cli launcher for application %app% > selftest_report.log
  goto :check_gui
)
type NUL > tmp.log
bin\otbcli_%app%.bat -help > tmp.log 2>&1
call :parse_cli_output
if %parse_cli_result% equ 0 (
  echo ERROR : error launching application otbcli_%app%
  echo ERROR : error launching application otbcli_%app% > selftest_report.log
  type tmp.log > selftest_report.log
)
:check_gui
if not exist bin\otbgui_%app%.bat (
  echo ERROR : missing gui launcher for application %app%
  echo ERROR : missing gui launcher for application %app% > selftest_report.log
  goto :check_application_end
)
if %app_count% geq 2 goto :check_application_end
type NUL > tmp.log
start /b bin\otbgui_%app%.bat
timeout 5
call :get_child_pid %CURRENT_PID% cmd.exe
set first_child=0
if %child_pid% gtr 1 (
  set first_child=%child_pid%
) else (
  echo ERROR : could not launch otbgui_%app%
  echo ERROR : could not launch otbgui_%app% > selftest_report.log
  goto :check_application_clean
)
call :get_child_pid %first_child% otbApplicationLauncherQt.exe
if not %child_pid% gtr 1 (
  echo ERROR : could not launch otbApplicationLauncherQt %app%
  echo ERROR : could not launch otbApplicationLauncherQt %app% > selftest_report.log
)
:check_application_clean
if not %first_child% == 0 (
  taskkill /PID %first_child% /F /T > NUL 2>&1
)
:check_application_end
endlocal & set /a app_count=app_count+1
goto :eof

:launch_gui
setlocal
start /b bin\otbgui_%app%.bat
endlocal
goto :eof

:parse_cli_output
setlocal
set ret=1
for /F %%a in ('findstr /n /r /c:"^This is the %app% application, version " tmp.log') do set output=%%a
if not "%output%" gtr "1:" (
  echo First line not found
  set ret=0
  goto :parse_cli_output_end
)
set output=
for /F %%a in ('findstr /n /r /c:"^Parameters:" tmp.log') do set output=%%a
if not "%output%" gtr "3:" (
  echo Parameters line not found
  set ret=0
  goto :parse_cli_output_end
)
set output=
for /F %%a in ('findstr /n /r /c:"^Examples:" tmp.log') do set output=%%a
if not "%output%" gtr "1" (
  echo Examples line not found
  echo %output%
  set ret=0
  goto :parse_cli_output_end
)
set output=
for /F %%a in ('findstr /n /r /c:"FATAL" tmp.log') do set output=%%a
if "%output%" gtr "1" (
  echo FATAL found
  echo %output%
  set ret=0
  goto :parse_cli_output_end
)
:parse_cli_output_end
endlocal & set parse_cli_result=%ret%
goto :eof

:nb_report_lines
setlocal
for /F "delims=" %%i in ('find /C /V "" ^< selftest_report.log') do set output=%%i
endlocal & set nb_report_lines_out=%output%
goto :eof

:nb_tmp_lines
setlocal
for /F "delims=" %%i in ('find /C /V "" ^< tmp.log') do set output=%%i
endlocal & set nb_tmp_lines_out=%output%
goto :eof

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
