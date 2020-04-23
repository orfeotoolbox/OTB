#!/bin/bash
#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

# usage : call CI/otb_coverage.sh from source dir
OTB_DIR="$(dirname $0)/.."
OTB_DIR="$(readlink -f $OTB_DIR)"

if [ -z "$BUILD_DIR" ]; then
BUILD_DIR=${OTB_DIR}/build
fi

echo Generating gcov reports in $BUILD_DIR ...
cd $BUILD_DIR
find $BUILD_DIR -name "*.gcda" -exec llvm-cov gcov -p '{}' > /dev/null \;
ls *.gcov | grep -E -v '#Modules#[a-zA-Z0-9]+#[a-zA-Z0-9]+#(include|src|app)#' | xargs -L 1 rm
echo Filtered $(ls $BUILD_DIR/*.gcov | wc -l) gcov reports

gcovr -r $OTB_DIR -x -g --object-directory=$BUILD_DIR > $BUILD_DIR/coverage_report.xml
echo Generated $BUILD_DIR/coverage_report.xml with $(grep -c '<class ' $BUILD_DIR/coverage_report.xml) classes
