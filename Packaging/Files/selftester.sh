#!/bin/sh
#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Setup test environment
CUR_DIR="$( cd "$( dirname "$0" )/../" && pwd )"
echo "CUR_DIR=$CUR_DIR"
cd "$CUR_DIR" || exit

EXIT_ON_ERROR=1

# define convenient functions
# ps_children( parentPID ) : get PIDs of children processes
ps_children () {
  ps -e -o pid -o command=COMMAND-WIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIDE-COLUMN -o ppid | grep " $1$" | grep -v -e ' grep ' -e ' ps -e '
}

# get the pid field from the output line of ps_children
get_pid() {
  echo "$1" | grep -E -o '[0-9]+' | head -n 1
}

# nb_report_lines( ) : get number of lines in report
nb_report_lines () {
  report_lines="$(wc -l selftest_report.log)"
  echo $report_lines | cut -d ' ' -f 1
}
#

exit_if () {
  if [ $EXIT_ON_ERROR = 1 ]; then
      echo "EXIT_ON_ERROR=1"
      exit 1;
  fi
}

# echo_and_report ( string ) : echo and print to report
echo_and_report () {
    echo "$1" | tee -a selftest_report.log
    exit_if
}

# -------------------------------------------------------------------------
. "$CUR_DIR/otbenv.profile"

rm -f selftest_report.log
touch selftest_report.log
REF_SIZE=0
REF_SIZE=$(nb_report_lines)

# Check 1 : check binaries
OTB_SO_LIBRARIES=$(find lib -name '*.so*' | grep -v 'python')
OTB_DY_LIBRARIES=$(find lib -name '*.dylib')
OTB_EXE="bin/otbApplicationLauncherCommandLine"
for name in $OTB_SO_LIBRARIES $OTB_DY_LIBRARIES $OTB_EXE; do
    F_OUTPUT=$(file "$name")
  if echo "$F_OUTPUT" | grep -q 'cannot open'; then
    echo_and_report "$F_OUTPUT"
  elif echo "$F_OUTPUT" | grep -q ': broken symbolic link'; then
    echo_and_report "$F_OUTPUT"
  elif echo "$F_OUTPUT" | grep -q -i -e ': ELF .*shared object' -e ': ELF .*executable'; then
    LDD_ERRORS=$(ldd "$name" 2>&1 | grep -v 'you do not have execution permission' | grep -i -e '=> not found' -e 'not a dynamic executable')
    if [ -n "$LDD_ERRORS" ]; then
      echo_and_report "ldd $name"
      echo_and_report "$LDD_ERRORS"
    fi
  elif echo "$F_OUTPUT" | grep -q -i -e ': Mach-O .*shared library' -e ': Mach-O .*bundle' -e ': Mach-O .*executable'; then
    DL_ERRORS=$(./tools/otb_loader "$name")
    if [ -n "$DL_ERRORS" ]; then
      echo_and_report "otb_loader $name"
      echo_and_report "$DL_ERRORS"
    fi
  elif echo "$F_OUTPUT" | grep -q ': symbolic link'; then
    :
  else
    echo_and_report "Unknown file type : $F_OUTPUT"
  fi
done

REPORT_SIZE=$(nb_report_lines)
if [ "$REPORT_SIZE" -ne "$REF_SIZE" ]; then
  echo "Check 1/3 : FAIL"
else
  echo "Check 1/3 : PASS"
fi
REF_SIZE=$REPORT_SIZE

# Check 2 : OTB applications and Python wrapping
OTB_APP_COUNT=$(find lib/otb/applications -name 'otbapp_*.*' | wc -w)
OTB_APPS=$(find lib/otb/applications -name 'otbapp_*.*' | cut -d '_' -f 2 | cut -d '.' -f 1)
if [ "$OTB_APP_COUNT" -le 90 ]; then
  echo "WARNING: Only $OTB_APP_COUNT applications found ! Expected at least 90"
fi
app_index=0
for app in $OTB_APPS; do
  if [ ! -f "bin/otbcli_$app" ]; then
      echo_and_report "ERROR: missing cli launcher for application $app"
      
  else
    CLI_OUTPUT=$("bin/otbcli_$app" -help 2>&1)
    CLI_FILTER=$(echo "${CLI_OUTPUT}"| tr '\n' ' ' | grep -E "This is the $app application, version .* Parameters: .* Examples:.*")
    CLI_FILTER2=$(echo "$CLI_FILTER" | grep -v 'FATAL')
    if [ -z "$CLI_FILTER2" ]; then
      echo_and_report "ERROR: bin/otbcli_$app\n$CLI_OUTPUT"
    fi
  fi
  app_index=$(( app_index + 1 ))
done

# test python wrapping
PY_OUTPUT=$(python3 -c 'import otbApplication' 2>&1)
if [ -n "$PY_OUTPUT" ]; then
  echo_and_report "ERROR: python wrapping test failed :"
  echo_and_report "$PY_OUTPUT"
fi

REPORT_SIZE=$(nb_report_lines)
if [ "$REPORT_SIZE" -ne "$REF_SIZE" ]; then
  echo "Check 2/3 : FAIL"
else
  echo "Check 2/3 : PASS"
fi
REF_SIZE=$REPORT_SIZE

grep_cmd=$(which grep)
grep_out=$($grep_cmd -Rs "/usr/" "$CUR_DIR/lib/cmake/" | $grep_cmd -v "/usr/include/libdrm")
grep_ret=$?
if [ $grep_ret -ne 1 ]; then
    echo "Check 3/3 : FAIL"
    echo "ERROR: your cmake files contains references to /usr. Is this normal?"
    echo_and_report "$grep_out"
else
    echo "Check 3/3 : PASS"
fi

# clean any background process
ps_children $$ >tmp.log
for pid in $(cut -d ' ' -f 1 < tmp.log); do
  kill -9 "$pid"
done
rm -f tmp.log
