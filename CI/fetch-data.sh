#!/bin/bash

# Fetch corresponding OTB-Data

curl -s -S -L -o otb-data-master.tar.gz \
     https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-data/-/archive/master/otb-data-master.tar.gz
tar xzf otb-data-master.tar.gz
mv otb-data-master otb-data
rm -f otb-data-master.tar.gz
