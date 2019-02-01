#!/bin/bash

cd build

# TODO compile tests

xvfb-run -a -n 1 -s "-screen 0 1024x768x24 -dpi 96" ctest -j 10 -T test
