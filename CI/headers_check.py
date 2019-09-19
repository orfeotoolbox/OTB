#!/usr/bin/python3
#
# -*- coding: utf-8 -*-
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

import os, re

# Root search directory
topdir = '.'

# Header to associate to each file extension
# - C++ and JavaScript comments are defined by /* ... */ (=> C++ header)
# - Shell, Python, Perl comments are defubed by # ... (=> Shell header)
# - Text files must be processed on a case-by-case basis according to their
#   content and what is done with them
# - It is complicated to insert a comment in an image file and most of them
#   were not created by the OTB project (=> no header)
# - There is no need to insert a copyright header in the generated files or
#   configuration files (=> no header)
# - Most of CSS files were not created by the OTB project (=> no header)
# - etc.
fileext = {
    '.cpp': 'cpp',
    '.cxx': 'cpp',
    '.txx': 'cpp',
    '.h': 'cpp',
    '.hpp': 'cpp',
    '.hxx': 'cpp',
    '.h.in': 'cpp',
    '.includes': 'cpp',
    '.i': 'cpp',
    '.js': 'cpp',
    '.sh': 'shell',
    '.sh.in': 'shell',
    '.bash': 'shell',
    '.profile': 'shell',
    '.py': 'shell',
    '.py.in': 'shell',
    '.cmake': 'shell',
    '.cmake.in': 'shell',
    '.yml': 'shell',
    '.pl': 'shell',
    '.bat': 'batch',
    '.bat.in': 'batch',
    '.png': 'none',
    '.jpg': 'none',
    '.tif': 'none',
    '.xpm': 'none',
    '.ico': 'none',
    '.eps': 'none',
    '.svg': 'none',
    '.txt': 'none',
    '.md': 'none',
    '.rst': 'none',
    '.xml': 'none',
    '.html': 'none',
    '.dox': 'none',
    '.dox.in': 'none',
    '.ts': 'none',
    '.ui': 'none',
    '.qrc': 'none',
    '.rc': 'none',
    '.rc.in': 'none',
    '.css': 'none'
}

specialfiles = {
    'CMakeLists.txt': 'shell',       # i.e. CMake file but Shell like comments
    'StandaloneWrapper.in': 'shell', # i.e. CMake file but Shell like comments
    'macx_pkgsetup.in': 'shell',
    'linux_pkgsetup.in': 'shell'
}

# Directories to exclude from the header checking for various reasons (third
# party works, data, patches, ...)
excludeddirs = set([
    './.git',
    './Data',
    './Modules/ThirdParty',
    './Packaging/makeself',
    './SuperBuild/Copyright',
    './SuperBuild/patches'
])

# Files to exclude from the header checking for various reasons (full text of
# licenses, binary archives, ...)
excludedfiles = set([
    './.clang-format',
    './.editorconfig',
    './.gitattributes',
    './.gitignore',
    './.mailmap',
    './sonar-project.properties',
    './LICENSE',
    './NOTICE',
    './VERSION',
    './CI/ctest2junit.xsl',
    './CI/test/README',
    './CMake/CppcheckTargets.cmake',
    './CMake/FindGLEW.cmake',
    './CMake/FindKWStyle.cmake',
    './CMake/FindLibSVM.cmake',
    './CMake/FindOpenThreads.cmake',
    './CMake/Findcppcheck.cmake',
    './CMake/Findcppcheck.cpp',
    './CMake/GenerateExportHeaderCustom.cmake',
    './CMake/InsightValgrind-RHEL6.supp',
    './CMake/InsightValgrind.supp',
    './CMake/OTB_CheckCCompilerFlag.cmake',
    './CMake/PythonCompile.py',
    './CMake/TopologicalSort.cmake',
    './CMake/exportheader.cmake.in',
    './CMake/pre-commit',
    './CMake/qt.conf.in',
    './Documentation/Cookbook/Art/residual_registration-figure.tex',
    './Documentation/Cookbook/rst/Makefile.in',
    './Documentation/Cookbook/rst/conf.py.in',
    './Modules/Visualization/Ice/README',
    './Modules/Wrappers/SWIG/src/numpy.i',
    './Packaging/Files/Monteverdi.icns',
    './Packaging/Files/OTB Project.zip',
    './Packaging/Files/qt.conf',
    './Packaging/Files/template.app/Contents/Info.plist',
    './Packaging/LICENSE',
    './Packaging/howto_update_makeself',
    './Packaging/otb_update_makeself',
    './SuperBuild/LICENSE',
    './Utilities/Data/Icons/LICENSE.TXT',
    './Utilities/Data/Icons/NOTES',
    './Utilities/Data/monteverdi.desktop.in',
    './Utilities/Data/monteverdi2.desktop',
    './Utilities/Doxygen/doxygen.config.in',
    './Utilities/Maintenance/BuildHeaderTest.py',
    './Utilities/Maintenance/fix_typos.sh'
])

regcppheader = re.compile(
r'''/\*
( \* Copyright \(C\) 1999-2011 Insight Software Consortium
| \* Copyright \(C\) 20\d\d(-20\d\d)? Centre National d'Etudes Spatiales \(CNES\)
| \* Copyright \(C\) 20\d\d(-20\d\d)? CS Systemes d'Information \(CS SI\)
| \* Copyright \(C\) 2007-2012 Institut Mines Telecom / Telecom Bretagne
| \* Copyright \(C\) 20\d\d(-20\d\d)? IRSTEA
| \* Copyright \(C\) 2008 Jan Wegner
| \* Copyright \(C\) 2007 Julien Radoux
)+ \*
 \* This file is part of Orfeo Toolbox
 \*
 \*     https://www\.orfeo-toolbox\.org/
 \*
 \* Licensed under the Apache License, Version 2\.0 \(the "License"\);
 \* you may not use this file except in compliance with the License\.
 \* You may obtain a copy of the License at
 \*
 \*     http://www\.apache\.org/licenses/LICENSE-2\.0
 \*
 \* Unless required by applicable law or agreed to in writing, software
 \* distributed under the License is distributed on an "AS IS" BASIS,
 \* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied\.
 \* See the License for the specific language governing permissions and
 \* limitations under the License\.
 \*/
''', flags=re.MULTILINE)

regshellheader = re.compile(
r'''(.*
)+(# Copyright \(C\) 1999-2011 Insight Software Consortium
|# Copyright \(C\) 20\d\d(-20\d\d)? Centre National d'Etudes Spatiales \(CNES\)
|# Copyright \(C\) 20\d\d(-20\d\d)? CS Systemes d'Information \(CS SI\)
|# Copyright \(C\) 20\d\d(-20\d\d)? IRSTEA
|# Copyright \(C\) 2007-2012 Institut Mines Telecom / Telecom Bretagne
)+#
# This file is part of Orfeo Toolbox
#
#     https://www\.orfeo-toolbox\.org/
#
# Licensed under the Apache License, Version 2\.0 \(the "License"\);
# you may not use this file except in compliance with the License\.
# You may obtain a copy of the License at
#
#     http://www\.apache\.org/licenses/LICENSE-2\.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied\.
# See the License for the specific language governing permissions and
# limitations under the License\.
''', flags=re.MULTILINE)

regbatchheader = re.compile(
r'''(.*
)+(:: Copyright \(C\) 1999-2011 Insight Software Consortium
|:: Copyright \(C\) 20\d\d(-20\d\d)? Centre National d'Etudes Spatiales \(CNES\)
|:: Copyright \(C\) 20\d\d(-20\d\d)? CS Systemes d'Information \(CS SI\)
|:: Copyright \(C\) 2007-2012 Institut Mines Telecom / Telecom Bretagne
)+::
:: This file is part of Orfeo Toolbox
::
::     https://www\.orfeo-toolbox\.org/
::
:: Licensed under the Apache License, Version 2\.0 \(the "License"\);
:: you may not use this file except in compliance with the License\.
:: You may obtain a copy of the License at
::
::     http://www\.apache\.org/licenses/LICENSE-2\.0
::
:: Unless required by applicable law or agreed to in writing, software
:: distributed under the License is distributed on an "AS IS" BASIS,
:: WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied\.
:: See the License for the specific language governing permissions and
:: limitations under the License\.
''', flags=re.MULTILINE)


debug = False
returnvalue = 0


def verifyheader(filename, category):

    global returnvalue

    if category == 'none':
        if debug: print('File type ignored ({} style comment): {}'.format(category, filename))
        return True

    sourceFile = open(filename)
    sourceContent = sourceFile.read().lstrip()
    sourceFile.close()

    if category == 'cpp':
        m = regcppheader.match(sourceContent)
    elif category == 'shell':
        m = regshellheader.match(sourceContent)
    elif category == 'batch':
        m = regbatchheader.match(sourceContent)
    else:
        print('WARNING: Unable to evaluate header ({} style comment): {}'.format(category, filename))
        returnvalue = 1
        return False

    if m:
        if debug: print('Conform header ({} style comment): {}'.format(category, filename))
        return True
    else:
        print('WARNING: Non-compliant header ({} style comment): {}'.format(category, filename))
        returnvalue = 1
        return False


# Regular expression catching the file extension (with patterns '.xxx' or
# '.xxx.yyy', like '.cmake' or '.cmake.in')
# NOTE: '+?' operator is the not greedy / hungry version of '+' operator.
extreg = re.compile(r'^.+?(\.[^.]+(\.[^.]+)?)$')

for root, dirs, files in os.walk(topdir, topdown=True):

    # "dirs[:]" modify dirs "in-place". In doing so, we can ignore some
    # directories (here, the directories listed in the 'excludeddirs' set.
    dirs[:] = [d for d in dirs if os.path.join(root, d) not in excludeddirs]

    for f in sorted(files):
        fullpathname = os.path.join(root, f)
        # 1. Exclusion of files in which we do not want to search for the
        #    copyright header.
        if fullpathname in excludedfiles:
            if debug: print('File excluded: {}'.format(fullpathname))
        # 2. Early identification of files with a misleading extension
        elif f in specialfiles.keys():
            verifyheader(fullpathname, specialfiles[f])
        # 3. Processing of other files according to their extension
        else:
            m = extreg.match(f)
            if m:
                # NOTE: m.group(0) = Whole string matched by the regular
                # expression (here, the whole name of the file)
                ext1 = m.group(1) # '.xxx' or '.xxx.yyy' form
                ext2 = m.group(2) # '.yyy' form (optional and included in group #1)

                # The test of two extensions (ext1, ext2) rather than only one
                # make possible to distinguish known extensions, such ".cmake"
                # or ".cmake.in", from patterns that look like a double
                # extension, but are not, like ".remote.cmake". So, the
                # appropriate processing can be applied (".remote.cmake" must
                # be identified as ".cmake" and not as an unknown extension).
                if ext1 in fileext.keys():
                    verifyheader(fullpathname, fileext[ext1])
                elif ext2 in fileext.keys():
                    verifyheader(fullpathname, fileext[ext2])
                else:
                    print('WARNING: File with an unknown extension: {}'.format(fullpathname))
                    returnvalue = 1
            else:
                print('WARNING: File without extension and not excluded: {}'.format(fullpathname))
                returnvalue = 1

exit(returnvalue)
