#!/usr/bin/env python3
#-*- coding: utf-8 -*-
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

import otbApplication as otb

# TO RUN WITH PYTHON3
# try:
#    unicode = unicode
# except NameError:
#    # 'unicode' is undefined, must be Python 3
#    str = str
#    unicode = str
#    bytes = bytes
#    basestring = (str,bytes)
# else:
#    # 'unicode' exists, must be Python 2
#    str = str
#    unicode = unicode
#    bytes = str
#    basestring = basestring
  

# This test checks that UpdateOutputInformation() has been called on output image pointers when connecting pipeline in-memory
# This code snippet has been used to reproduce bug #1498

if __name__ == '__main__':
    inimage = sys.argv[2]
    outimage = sys.argv[3]

    ext1 = otb.Registry.CreateApplication("ExtractROI")
    ext2 = otb.Registry.CreateApplication("ExtractROI")

    ext1.SetParameterString('in',inimage)
    ext1.Execute()
    ext1.UpdateParameters()
    
    ext2.SetParameterInputImage('in',ext2.GetParameterOutputImage('out'))
    ext2.UpdateParameters()
    ext2.SetParameterStringList('cl',['Channel1'])
    ext2.SetParameterString("out",outimage)
    ext2.ExecuteAndWriteOutput()
