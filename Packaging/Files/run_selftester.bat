@echo on
set CURDIR=%~dp0
set EXIT_ON_ERROR=1
set VERBOSE=0
cmd /k %CURDIR%tools\selftester.bat /q > %CURDIR%selftest_report.log 2>&1