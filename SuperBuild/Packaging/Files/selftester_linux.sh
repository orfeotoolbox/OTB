#!/bin/sh

# Setup test environment
DIRNAME_0=$(dirname "$0")
DIRNAME=$(readlink -f "$DIRNAME_0/..")
cd "$DIRNAME"

. ./otbenv.profile

rm -f selftest_report.log
touch selftest_report.log
REF_SIZE=0
REF_SIZE=$(wc -l selftest_report.log | cut -d ' ' -f 1)

# Check 1 : check binaries
OTB_LIBRARIES="$(ls lib/lib*.so*) $(ls lib/otb/applications/otbapp_*.so) lib/python/_otbApplication.so"
OTB_EXE="bin/mapla bin/monteverdi bin/otbApplicationLauncherQt bin/otbApplicationLauncherCommandLine"
for name in $OTB_LIBRARIES $OTB_EXE ; do
  F_OUTPUT=$(file $name)
  if [ -n "$(echo $F_OUTPUT | grep 'cannot open')" ]; then
    echo "$F_OUTPUT" | tee -a selftest_report.log
  elif [ -n "$(echo $F_OUTPUT | grep ': broken symbolic link')" ]; then
    echo "$F_OUTPUT" | tee -a selftest_report.log
  elif [ -n "$(echo $F_OUTPUT | grep -e ': ELF .*shared object' -e ': ELF .*executable')" ]; then
    LDD_ERRORS=$(ldd $name | grep -e '=> not found' -e 'not a dynamic executable')
    if [ -n "$LDD_ERRORS" ]; then
      echo "ldd $name" | tee -a selftest_report.log
      echo "$LDD_ERRORS" | tee -a selftest_report.log
    fi
  fi
done

REPORT_SIZE=$(wc -l selftest_report.log | cut -d ' ' -f 1)
if [ $REPORT_SIZE -ne $REF_SIZE ]; then
  echo "Check 1/3 : FAIL"
  exit 1
fi
echo "Check 1/3 : PASS"
REF_SIZE=$(wc -l selftest_report.log | cut -d ' ' -f 1)

# Check 2 : OTB applications and Python wrapping
OTB_APP_COUNT=$(ls lib/otb/applications/otbapp_*.so | wc -w)
OTB_APPS=$(ls lib/otb/applications/otbapp_*.so | cut -d '_' -f 2 | cut -d '.' -f 1)
if [ $OTB_APP_COUNT -le 90 ]; then
  echo "WARNING: Only $OTB_APP_COUNT applications found ! Expected at least 90"
fi
for app in $OTB_APPS; do
  if [ ! -f "bin/otbcli_$app" ]; then
    echo "ERROR: missing cli launcher for application $app" | tee -a selftest_report.log
  else
    CLI_OUTPUT=$(bin/otbcli_$app -help 2>&1)
    CLI_FILTER=$(echo $CLI_OUTPUT | grep -E "^This is the $app application, version.* Parameters:( (MISSING )?-[0-9a-z]+ .*)+ Examples:.*otbcli_$app.*")
    CLI_FILTER2=$(echo $CLI_FILTER | grep -v 'FATAL')
    if [ -z "$CLI_FILTER2" ]; then
      echo "ERROR: bin/otbcli_$app" | tee -a selftest_report.log
      echo "$CLI_OUTPUT" | tee -a selftest_report.log
    fi
  fi
  
  if [ ! -f "bin/otbgui_$app" ]; then
    echo "ERROR: missing gui launcher for application $app" | tee -a selftest_report.log
  else
    echo "" >tmp.log
    bin/otbgui_$app 2>&1 >tmp.log &
    GUI_PID=$!
    sleep 1s
    # Check process tree
    CHILD_PROC=$(ps -o pid -o cmd --ppid $GUI_PID | grep -v 'PID *CMD' | grep "bin/otbgui $app")
    if [ -n "$CHILD_PROC" ]; then
      CHILD_PID=$(echo $CHILD_PROC | cut -d ' ' -f 1)
      NEXT_CHILD_PROC=$(ps -o pid -o cmd --ppid $CHILD_PID | grep -v 'PID *CMD' | grep 'otbApplicationLauncherQt')
      if [ -n "$NEXT_CHILD_PROC" ]; then
        NEXT_CHILD_PID=$(echo $NEXT_CHILD_PROC | cut -d ' ' -f 1)
        kill -9 $GUI_PID
        kill -9 $CHILD_PID
        kill -9 $NEXT_CHILD_PID
      else
        echo "ERROR: bin/otbgui_$app failed to launch" | tee -a selftest_report.log
        cat tmp.log | tee -a selftest_report.log
      fi
    else
      echo "ERROR: bin/otbgui_$app failed to launch" | tee -a selftest_report.log
      cat tmp.log | tee -a selftest_report.log
    fi
  fi
done

REPORT_SIZE=$(wc -l selftest_report.log | cut -d ' ' -f 1)
if [ $REPORT_SIZE -ne $REF_SIZE ]; then
  echo "Check 2/3 : FAIL"
else
  echo "Check 2/3 : PASS"
fi
REF_SIZE=$(wc -l selftest_report.log | cut -d ' ' -f 1)

# Check 3 : OTB binaries monteverdi & mapla
# TODO

REPORT_SIZE=$(wc -l selftest_report.log | cut -d ' ' -f 1)
if [ $REPORT_SIZE -ne $REF_SIZE ]; then
  echo "Check 3/3 : FAIL"
else
  echo "Check 3/3 : PASS"
fi

# TODO : clean any background process

