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

import sys
import argparse

"""
Originally reported on gitlab as bug #1899:
https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues/1899
"""

def test(otb, argv):
    input_img = argv[1]
    output_img = argv[2]

    ROI_1 = otb.Registry.CreateApplication("ExtractROI")
    ROI_1.SetParameterString("in", input_img)
    ROI_1.SetParameterString("startx", "0")
    ROI_1.SetParameterString("starty", "0")
    ROI_1.SetParameterString("sizex", "200")
    ROI_1.SetParameterString("sizey", "200")
    ROI_1.SetParameterString("out", output_img)
    ROI_1.Execute()

    ROI_2 = otb.Registry.CreateApplication("ExtractROI")
    ROI_2.SetParameterString("in", input_img)
    ROI_2.SetParameterString("startx", "0")
    ROI_2.SetParameterString("starty", "0")
    ROI_2.SetParameterString("sizex", "200")
    ROI_2.SetParameterString("sizey", "200")
    ROI_2.SetParameterString("out", output_img)
    ROI_2.Execute()

    band_math_1 = otb.Registry.CreateApplication("BandMath")
    band_math_1.SetParameterStringList("il", [input_img])
    band_math_1.SetParameterString("exp", "im1b1 + 1")
    band_math_1.SetParameterString("out", "")
    band_math_1.Execute()

    concatenate = otb.Registry.CreateApplication("ConcatenateImages")
    concatenate.AddImageToParameterInputImageList("il", ROI_1.GetParameterOutputImage("out"))
    concatenate.AddImageToParameterInputImageList("il", ROI_2.GetParameterOutputImage("out"))
    concatenate.AddImageToParameterInputImageList("il", band_math_1.GetParameterOutputImage("out"))
    concatenate.SetParameterString("out", output_img)
    concatenate.ExecuteAndWriteOutput()
