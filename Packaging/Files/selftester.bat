@echo off
setlocal

set CURRENT_SCRIPT_DIR=%~dp0
set DIRNAME=%CURRENT_SCRIPT_DIR%..
cd %DIRNAME%

:: if "/q" is given, hide the error dialogs (hence make the script non-interactive)
if /i not -%1-==-/q- (
  goto :skip_hide_error
)
echo Hide error dialogs
call :getDontShowUI
if %DONTSHOWUI_VALUE% equ 0 call :setDontShowUI 1
:skip_hide_error

:: Get current PID
type NUL > pid.log
wmic PROCESS where "Name='WMIC.exe'" get ParentProcessId >pid.log 2>&1
for /F "TOKENS=1" %%b in ('type pid.log ^| findstr [0-9]') do set CURRENT_PID=%%b

:: setup the otbenv
call otbenv.bat

del selftest_report.log
type NUL > selftest_report.log
call :nb_report_lines
set REF_SIZE=%nb_report_lines_out%
:: -----------------------------------------------------------------------------------
rem Check 1 : check binaries
for /R %%i in (*.dll) do (
  call :check_binary %%i
)
for %%i in (bin\mapla.exe,bin\monteverdi.exe,bin\otbApplicationLauncherCommandLine.exe,bin\otbApplicationLauncherQt.exe) do (
  call :check_binary %%i
)
call :check_binary lib\python\_otbApplication.pyd

call :nb_report_lines
if %nb_report_lines_out% gtr %REF_SIZE% (
  echo Check 1/3 : FAIL
) else (
  echo Check 1/3 : PASS
)
set REF_SIZE=%nb_report_lines_out%
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

call :nb_report_lines
if %nb_report_lines_out% gtr %REF_SIZE% (
  echo Check 2/3 : FAIL
) else (
  echo Check 2/3 : PASS
)
set REF_SIZE=%nb_report_lines_out%
:: -----------------------------------------------------------------------------------
rem Check 3 : check monteverdi & mapla
call :check_desktop_app monteverdi
call :check_desktop_app mapla 20

call :nb_report_lines
if %nb_report_lines_out% gtr %REF_SIZE% (
  echo Check 3/3 : FAIL
) else (
  echo Check 3/3 : PASS
)

if /i not -%1-==-/q- (
  goto :skip_restore_error
)
if %DONTSHOWUI_VALUE% equ 0 call :setDontShowUI 0
:skip_restore_error
del pid.log
del tmp.log
endlocal
goto :eof

::########################[ FUNCTIONS ]##################################
:check_binary
setlocal
type NUL > tmp.log
tools\otb_loader.exe %1 > tmp.log 2>&1
call :nb_tmp_lines
if %nb_tmp_lines_out% gtr 0 (
  echo ERROR : otb_loader.exe %1
  echo otb_loader.exe %1 >> selftest_report.log
  type tmp.log >> selftest_report.log
)
endlocal
goto :eof
::-----------------------------------------------------------------------
:check_application
setlocal
set dll_path=%~n1
set app=%dll_path:~7%
if not exist bin\otbcli_%app%.bat (
  echo ERROR : missing cli launcher for application %app%
  echo ERROR : missing cli launcher for application %app% >> selftest_report.log
  goto :check_gui
)
type NUL > tmp.log
cmd /C bin\otbcli_%app%.bat "-help" ^> tmp.log ^2^>^&^1
call :parse_cli_output
if %parse_cli_result% equ 0 (
  echo ERROR : error launching application otbcli_%app%
  echo ERROR : error launching application otbcli_%app% >> selftest_report.log
  type tmp.log >> selftest_report.log
)
:check_gui
if not exist bin\otbgui_%app%.bat (
  echo ERROR : missing gui launcher for application %app%
  echo ERROR : missing gui launcher for application %app% >> selftest_report.log
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
  echo ERROR : could not launch otbgui_%app% >> selftest_report.log
  type tmp.log >> selftest_report.log
  goto :check_application_clean
)
call :get_child_pid %first_child% otbApplicationLauncherQt.exe
if not %child_pid% gtr 1 (
  echo ERROR : could not launch otbApplicationLauncherQt %app%
  echo ERROR : could not launch otbApplicationLauncherQt %app% >> selftest_report.log
  type tmp.log >> selftest_report.log
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
  echo ERROR : could not launch %appName%.exe >> selftest_report.log
  type tmp.log >> selftest_report.log
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
  echo ERROR : failed to run python wrapping >> selftest_report.log
  type tmp.log >> selftest_report.log
)
endlocal
goto :eof
::-----------------------------------------------------------------------
:parse_cli_output
setlocal
set /a ret=1
for /F %%a in ('findstr /n /r /c:"^This is the %app% application, version " tmp.log') do set output=%%a
if not "%output%" gtr "1:" (
  set /a ret=0
  goto :parse_cli_output_end
)
set output=
for /F %%a in ('findstr /n /r /c:"^Parameters:" tmp.log') do set output=%%a
if not "%output%" gtr "3:" (
  set /a ret=0
  goto :parse_cli_output_end
)
set output=
for /F %%a in ('findstr /n /r /c:"^Examples:" tmp.log') do set output=%%a
if not "%output%" gtr "1" (
  set /a ret=0
  goto :parse_cli_output_end
)
set output=
for /F %%a in ('findstr /n /r /c:"FATAL" tmp.log') do set output=%%a
if "%output%" gtr "1" (
  set /a ret=0
  goto :parse_cli_output_end
)
:parse_cli_output_end
endlocal & set /a parse_cli_result=%ret%
goto :eof
::-----------------------------------------------------------------------
:nb_report_lines
setlocal
for /F "delims=" %%i in ('find /C /V "" ^< selftest_report.log') do set output=%%i
endlocal & set nb_report_lines_out=%output%
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
