#!/bin/bash

# usage : call CI/otb_coverage.sh from source dir
OTB_DIR="$(dirname $0)/.."
OTB_DIR="$(readlink -f $OTB_DIR)"

if [ -z "$BUILD_DIR" ]; then
BUILD_DIR=${OTB_DIR}/build
fi

ls -lh $BUILD_DIR/Modules/Core/Common/src/CMakeFiles/OTBCommon.dir/otbLogger.cxx.gcda

cd $BUILD_DIR
mkdir cov_filter

echo Generating gcov reports in $BUILD_DIR ...
find $BUILD_DIR -name "*.gcda" -exec llvm-cov gcov -p '{}' > /dev/null \; 
find . -maxdepth 1 -regex '.*#Modules#[a-zA-Z0-9]+#[a-zA-Z0-9]+#\(include\|src\|app\)#.*\.gcov' -exec mv '{}' cov_filter \;
rm *.gcov
cd $OTB_DIR

echo Filtered $(ls $BUILD_DIR/cov_filter | wc -l) gcov reports
du -sh cov_filter

echo Generating $BUILD_DIR/coverage_report.xml ...

gcovr -r $OTB_DIR -x -g -k --object-directory=$BUILD_DIR/cov_filter > $BUILD_DIR/coverage_report.xml

ls -lh $BUILD_DIR/coverage_report.xml
