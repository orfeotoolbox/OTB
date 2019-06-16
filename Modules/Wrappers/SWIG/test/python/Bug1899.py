#!/usr/bin/env python3
#-*- coding: utf-8 -*-

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
