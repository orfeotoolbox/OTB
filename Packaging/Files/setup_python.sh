#!/bin/sh
#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

# get current working dir (this should be the install directory)
CWD=$(pwd)

# Setup python environment
if [ ! -f "$OTB_PYTHON_EXE" ] ; then
    OTB_PYTHON_EXE=$(which python)
fi

if [ ! -f "$OTB_PYTHON_EXE" ] ; then
    # On old system we still have python => python2.x and python3 => python3.x
    OTB_PYTHON_EXE=$(which python3)
    if [ ! -f "$OTB_PYTHON_EXE" ] ; then
      printf %s\\n "*****Error occurred during installation******"
      printf %s\\n "Python executable not found"
      exit 1
    fi
fi

python_version_check() {
  python_major_version=$($OTB_PYTHON_EXE -c "import sys;print(sys.version_info[0])")
  python_minor_version=$($OTB_PYTHON_EXE -c "import sys;print(sys.version_info[1])")
  python_patch_version=$($OTB_PYTHON_EXE -c "import sys;print(sys.version_info[2])")
  python_version="$python_major_version.$python_minor_version.$python_patch_version"
}

python_check_failed() {
    printf %s\\n "*****Error occurred during installation******"
    printf %s\\n "OTB python bindings require python3 but current detected python version is $python_version"
    printf %s\\n "If you have python3 installed in your system "
    printf %s\\n "You should set OTB_PYTHON_EXE and re-run this installation script."
    printf %s\\n "eg: OTB_PYTHON_EXE=/path/to/python3 ./OTB-X.Y-Linux64.run"
    exit 1
}
# Do we need the exact version of python?
if [ "$python_major_version" -lt 3 ]; then
  OTB_PYTHON_EXE=$(which python3)
  if [! -f "$OTB_PYTHON_EXE"] ; then
    python_check_failed
  else
    python_version_check()
    if [ "$python_major_version" -lt 3 ]; then
      python_check_failed
    fi
  fi
fi

# test for 3.5 only ! and warn user if not!
if [ "$python_major_version" -eq 5 ]; then
  printf %s\\n "*****Warning******"
  printf %s\\n "OTB python bindings require python3.5 but current detected python version is $python_version"
  printf %s\\n "This case is undefined!"
fi

python_INSTSONAME=$($OTB_PYTHON_EXE -c "import sys; from distutils import sysconfig; print (sysconfig.get_config_var('INSTSONAME'));")

python_lib_dirs="$LD_LIBRARY_PATH /usr/lib /usr/lib64 /usr/lib/x86_64-linux-gnu"
if [ "$(uname)" = "Darwin" ]; then
    python_lib_dirs="$DYLD_LIBRARY_PATH /usr/lib /Library/Frameworks /opt/local/lib /opt/local/Library/Frameworks"
fi;

found_python_lib="0"
python_lib_file_path=""
for list_of_dir in $python_lib_dirs
do
    if [ -f "$list_of_dir/$python_INSTSONAME" ]; then
        python_lib_file_path="$list_of_dir/$python_INSTSONAME"
        found_python_lib="1"
        break
    fi
done

if [ "$found_python_lib" -eq "1" ]; then
    numpy_import_result="$($OTB_PYTHON_EXE -c 'import numpy' 2>&1)" || numpy_ret=$?
    if [ ! -z "$numpy_import_result" ]; then
      printf %s\\n "*****Error occurred during installation******"
      printf %s\\n "Python interpreter detected is : $OTB_PYTHON_EXE ( version: $python_version )"
      printf %s\\n "numpy not installed with '$OTB_PYTHON_EXE'"
      printf %s\\n "Check failed with result:"
      printf %s\\n "$numpy_import_result"
      exit 1
    else
      printf %s\\n "OTB python bindings will be configured for $OTB_PYTHON_EXE ( version: $python_version )"
      printf %s\\n "Found python library: $python_lib_file_path"
    fi
    #ln -sf "$python_lib_file_path" "$CWD/lib/$python_INSTALLED_SONAME"
else
    printf %s\\n "*****Error occurred during installation******"
    printf %s\\n "Python interpreter detected is : $OTB_PYTHON_EXE ( version: $python_version )"
    printf %s\\n "$python_INSTSONAME cannot be found in any of search directories."
    printf %s\\n "We had searched following directories $python_lib_dirs"
    printf %s\\n "If you don't have python-dev package installed, install it and make a symlink"
    printf %s\\n "If you don't have python headers and so installed on a custom location, then make a symlink"
    printf %s\\n "eg: ln -s /usr/lib/x86_64-linux-gnu/$python_INSTSONAME $CWD/lib/$python_INSTSONAME"
    exit 1
fi
