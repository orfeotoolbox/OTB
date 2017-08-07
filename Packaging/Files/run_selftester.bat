@echo on
set CURDIR=%~dp0
set EXIT_ON_ERROR=0
set VERBOSE=1
cmd /k %CURDIR%tools\selftester.bat /q > %CURDIR%selftest_report.log 2>&1
