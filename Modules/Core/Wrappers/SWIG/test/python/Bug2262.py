# -*- coding: utf-8 -*-
#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

"""
Originally reported on gitlab as bug #2262:
https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues/2262
"""

def test(otbApplication, argv):
    bandmath1 = otbApplication.Registry.CreateApplication("BandMath")
    bandmath1.SetParameterStringList("il", [argv[1]])
    bandmath1.SetParameterString("exp", "im1b1")
    bandmath1.Execute()

    bandmath2 = otbApplication.Registry.CreateApplication("BandMath")
    bandmath2.ImportImage("il", bandmath1.ExportImage("out"))
    bandmath2.SetParameterString("exp", "im1b1+1")
    bandmath2.Execute()

    with open(argv[2], 'w') as outfile:
        outfile.write("Test Bug2262.py\n")
        outfile.write(bandmath2.GetImageProjection("out"))

