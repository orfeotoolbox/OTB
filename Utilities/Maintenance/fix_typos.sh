#!/bin/sh
# -*- coding: utf-8 -*-

###############################################################################
#  Copyright (c) 2016, Even Rouault <even.rouault at spatialys.com>
#
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  and/or sell copies of the Software, and to permit persons to whom the
#  Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included
#  in all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
#  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#  DEALINGS IN THE SOFTWARE.
###############################################################################

#Portion of the script is based on fix_typos.sh in gdal source

#Install codespell (pip install codespell or git https://github.com/lucasdemarchi/codespell.git)
# Note GDAL is using a patched version to support white list and full filename for exclude files

# Aggregate base dictionary + QGIS one + Debian Lintian one (same as in gdal)
curl https://raw.githubusercontent.com/qgis/QGIS/master/scripts/spelling.dat | sed "s/:/->/" | grep -v "colour->" | grep -v "colours->" > qgis.txt
curl https://anonscm.debian.org/cgit/lintian/lintian.git/plain/data/spelling/corrections| grep "||" | grep -v "#" | sed "s/||/->/" > debian.txt
cat codespell/data/dictionary.txt qgis.txt debian.txt | awk 'NF' > otb_dict.txt

#Build exclude files and directory list
EXCLUDED_FILES="*/.git*
#Exclude third parties code, patches from superbuild and doxygen files
EXCLUDED_FILES="$EXCLUDED_FILES,*/Modules/ThirdParty/6S/*,*/Modules/ThirdParty/SiftFast/*,*/SuperBuild/patches/*,*/Utilities/Doxygen/*,"
#Exclude also documentation for now
EXCLUDED_FILES="$EXCLUDED_FILES,*/Documentation/*"
#Exclude also Copyright folder for now
EXCLUDED_FILES="$EXCLUDED_FILES,*/Copyright/*,*/SuperBuild/Copyright/*,*/Utilities/Maintenance/*"

codespell.py -w -i 3 -q 2 -S $EXCLUDED_FILES \
             -D otb_dict.txt ../..
