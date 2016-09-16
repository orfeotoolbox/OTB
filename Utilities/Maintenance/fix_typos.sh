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

#FIXME
#Rename AssymetricFusionOfDetectorImageFilter to AsymetricFusionOfDetectorImageFilter
#Rename Restauration to Restoration
#Rename InstanciateTransform() to InstantiateTransform()
#Rename Adressing  ==> Addressing
# ../../Modules/Core/Metadata/include/otbDefaultImageMetadataInterface.h:155: clas  ==> class  | disabled because of name clash in c++
# m_ShrinkedOutput -> m_ShrunkOutput
# convertor -> converter

if ! test -d fix_typos; then
    # Get even rouault fork of codespell that adds --words-white-list and full filename support for -S option
    mkdir fix_typos
    cd fix_typos
    git clone https://github.com/rouault/codespell
    cd codespell
    git checkout gdal_improvements
    cd ..
    # Aggregate base dictionary + QGIS one + Debian Lintian one
    curl https://raw.githubusercontent.com/qgis/QGIS/master/scripts/spelling.dat | sed "s/:/->/" | grep -v "colour->" | grep -v "colours->" > qgis.txt
    curl https://anonscm.debian.org/cgit/lintian/lintian.git/plain/data/spelling/corrections| grep "||" | grep -v "#" | sed "s/||/->/" > debian.txt
    cat codespell/data/dictionary.txt qgis.txt debian.txt | awk 'NF' > otb_dict.txt
    cd ..
fi

#Build exclude files and directory list
EXCLUDED_FILES="*/.git*"
#Exclude third parties code, patches from superbuild and doxygen files
EXCLUDED_FILES="$EXCLUDED_FILES,*/Modules/ThirdParty/6S/*,*/Modules/ThirdParty/SiftFast/*,*/Modules/ThirdParty/SPTW/*,*/SuperBuild/patches/*,*/Utilities/Doxygen/*"
#Exclude also documentation for now
EXCLUDED_FILES="$EXCLUDED_FILES,*/Documentation/*"
#Exclude also Copyright folder for now
EXCLUDED_FILES="$EXCLUDED_FILES,*/Copyright/*,*/SuperBuild/Copyright/*,*/Utilities/Maintenance/*,LICENSE,*/i18n/*"

#We use also words white list to
#This list should be updated after each release when deprecated classes/methods are removed
# use with --words-white-list=$WORDS_WHITE_LIST

# WORDS_WHITE_LIST="Assymmetric"
# WORDS_WHITE_LIST="$WORDS_WHITE_LIST,Instanciate"
# WORDS_WHITE_LIST="$WORDS_WHITE_LIST,Initialisation"

#FIXME need to add all deprecated classes and methods with typos in the white list



python3 fix_typos/codespell/codespell.py codespell.py -w -i 3 -q 2 -S $EXCLUDED_FILES -D fix_typos/otb_dict.txt  ../..
