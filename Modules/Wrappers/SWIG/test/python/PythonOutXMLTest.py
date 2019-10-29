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

# -*- coding: utf-8 -*-

#
#  Example on the use of the outxml via BandMath
#

def test(otb, argv):
    imagelist = argv[1:-2]
    exp = "cos(im1b1)+im2b1*im3b1-im3b2+ndvi(im3b3, im3b4)"
    out = argv[-2]
    outxml = argv[-1]

    app = otb.Registry.CreateApplication('BandMath')
    app.SetParameterStringList("il", imagelist, True)
    app.SetParameterString("out", out, True)
    app.SetParameterString("exp", exp, True)

    app.SaveParametersToXML(outxml)

    app.ExecuteAndWriteOutput()
