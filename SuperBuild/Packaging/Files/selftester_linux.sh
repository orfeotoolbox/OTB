#!/bin/sh

# Setup test environment
DIRNAME_0=$(dirname "$0")
DIRNAME=$(readlink -f "$DIRNAME_0/..")
cd "$DIRNAME"

# define convenient functions
# ps_children( parentPID ) : get PIDs of children processes
ps_children () {
  ps -e -o pid -o command=COMMAND-WIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIDE-COLUMN -o ppid | grep " $1$" | grep -v -e ' grep ' -e ' ps -e '
}

# nb_report_lines( ) : get number of lines in report
nb_report_lines () {
  local res="$(wc -l selftest_report.log)"
  echo $res | cut -d ' ' -f 1
}

# echo_and_report ( string ) : echo and print to report
echo_and_report () {
  echo $1 | tee -a selftest_report.log
}

# -------------------------------------------------------------------------
. ./otbenv.profile

rm -f selftest_report.log
touch selftest_report.log
REF_SIZE=0
REF_SIZE=$(nb_report_lines)

# Check 1 : check binaries
OTB_LIBRARIES="$(ls lib/lib*.so*) $(ls lib/otb/applications/otbapp_*.so) lib/python/_otbApplication.so"
OTB_EXE="bin/mapla bin/monteverdi bin/otbApplicationLauncherQt bin/otbApplicationLauncherCommandLine"
for name in $OTB_LIBRARIES $OTB_EXE ; do
  F_OUTPUT=$(file $name)
  if [ -n "$(echo $F_OUTPUT | grep 'cannot open')" ]; then
    echo_and_report "$F_OUTPUT"
  elif [ -n "$(echo $F_OUTPUT | grep ': broken symbolic link')" ]; then
    echo_and_report "$F_OUTPUT"
  elif [ -n "$(echo $F_OUTPUT | grep -e ': ELF .*shared object' -e ': ELF .*executable')" ]; then
    LDD_ERRORS=$(ldd $name | grep -e '=> not found' -e 'not a dynamic executable')
    if [ -n "$LDD_ERRORS" ]; then
      echo_and_report "ldd $name"
      echo_and_report "$LDD_ERRORS"
    fi
  fi
done

REPORT_SIZE=$(nb_report_lines)
if [ $REPORT_SIZE -ne $REF_SIZE ]; then
  echo "Check 1/3 : FAIL"
  exit 1
fi
echo "Check 1/3 : PASS"
REF_SIZE=$REPORT_SIZE

# Check 2 : OTB applications and Python wrapping
OTB_APP_COUNT=$(ls lib/otb/applications/otbapp_*.so | wc -w)
OTB_APPS=$(ls lib/otb/applications/otbapp_*.so | cut -d '_' -f 2 | cut -d '.' -f 1)
if [ $OTB_APP_COUNT -le 90 ]; then
  echo "WARNING: Only $OTB_APP_COUNT applications found ! Expected at least 90"
fi
app_index=0
for app in $OTB_APPS; do
  if [ ! -f "bin/otbcli_$app" ]; then
    echo_and_report "ERROR: missing cli launcher for application $app"
  else
    CLI_OUTPUT=$(bin/otbcli_$app -help 2>&1)
    CLI_FILTER=$(echo $CLI_OUTPUT | grep -E "^This is the $app application, version.* Parameters:( (MISSING )?-[0-9a-z]+ .*)+ Examples:.*otbcli_$app.*")
    CLI_FILTER2=$(echo $CLI_FILTER | grep -v 'FATAL')
    if [ -z "$CLI_FILTER2" ]; then
      echo_and_report "ERROR: bin/otbcli_$app"
      echo_and_report "$CLI_OUTPUT"
    fi
  fi
  # test the gui launcher only on 2 first applications
  if [ ! -f "bin/otbgui_$app" ]; then
    echo_and_report "ERROR: missing gui launcher for application $app"
  elif [ $app_index -lt 2 ]; then
    echo "" >tmp.log
    bin/otbgui_$app 2>&1 >tmp.log &
    GUI_PID=$!
    sleep 1s
    # Check process tree
    CHILD_PROC=$(ps_children $GUI_PID | grep "bin/otbgui $app")
    if [ -n "$CHILD_PROC" ]; then
      CHILD_PID=$(echo $CHILD_PROC | cut -d ' ' -f 1)
      NEXT_CHILD_PROC="$(ps_children $CHILD_PID | grep 'otbApplicationLauncherQt')"
      if [ -n "$NEXT_CHILD_PROC" ]; then
        NEXT_CHILD_PID=$(echo $NEXT_CHILD_PROC | cut -d ' ' -f 1)
        kill -9 $GUI_PID
        kill -9 $CHILD_PID
        kill -9 $NEXT_CHILD_PID
      else
        echo_and_report "ERROR: otbApplicationLauncherQt $app failed to launch"
        cat tmp.log | tee -a selftest_report.log
      fi
    else
      echo_and_report "ERROR: bin/otbgui_$app failed to launch"
      cat tmp.log | tee -a selftest_report.log
    fi
  fi
  app_index=$(( app_index + 1 ))
done

# test python wrapping
PY_OUTPUT=$(python -c 'import otbApplication' 2>&1)
if [ -n "$PY_OUTPUT" ]; then
  echo_and_report "ERROR: python wrapping test failed :"
  echo_and_report "$PY_OUTPUT"
fi

REPORT_SIZE=$(nb_report_lines)
if [ $REPORT_SIZE -ne $REF_SIZE ]; then
  echo "Check 2/3 : FAIL"
else
  echo "Check 2/3 : PASS"
fi
REF_SIZE=$REPORT_SIZE

# Check 3 : OTB binaries monteverdi & mapla
# Monteverdi
echo "" >tmp.log
bin/monteverdi 2>&1 >tmp.log &
MVD_PID=$!
sleep 5s
MVD_PROC=$(ps -o pid -o command -p $MVD_PID | grep -v 'PID *COMMAND' | grep "monteverdi")
if [ -n "$MVD_PROC" ]; then
  MVD_LOG=$(grep -i -e 'error' -e 'exception' tmp.log)
  if [ -n "$MVD_LOG" ]; then
    echo_and_report "ERROR: launching monteverdi"
    cat tmp.log | tee -a selftest_report.log
  fi
  kill -9 $MVD_PID
else
  echo_and_report "ERROR: failed to launch monteverdi"
  cat tmp.log | tee -a selftest_report.log
fi
# Mapla
echo "" >tmp.log
bin/mapla 2>&1 >tmp.log &
MAPLA_PID=$!
sleep 5s
MAPLA_PROC=$(ps -o pid -o command -p $MAPLA_PID | grep -v 'PID *COMMAND' | grep "mapla")
if [ -n "$MAPLA_PROC" ]; then
  MAPLA_LOG=$(grep -i -e 'error' -e 'exception' tmp.log)
  if [ -n "$MAPLA_LOG" ]; then
    echo_and_report "ERROR: launching mapla"
    cat tmp.log | tee -a selftest_report.log
  fi
  kill -9 $MAPLA_PID
else
  echo_and_report "ERROR: failed to launch mapla"
  cat tmp.log | tee -a selftest_report.log
fi

REPORT_SIZE=$(nb_report_lines)
if [ $REPORT_SIZE -ne $REF_SIZE ]; then
  echo "Check 3/3 : FAIL"
else
  echo "Check 3/3 : PASS"
fi

# clean any background process
ps_children $$ >tmp.log
for pid in $(cat tmp.log | cut -d ' ' -f 1); do
  kill -9 $pid
done
rm -f tmp.log
