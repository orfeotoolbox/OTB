#!/bin/bash

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
