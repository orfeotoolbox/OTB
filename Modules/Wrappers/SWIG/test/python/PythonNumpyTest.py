# -*- coding: utf-8 -*-
#
# Copyright (C) 2005-2019 CS Systemes d'Information (CS SI)
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

#  Example on testing numpy
#


import os
#from scipy import misc

import numpy as np

def test(otbApplication, argv):
	#from PIL import Image as PILImage

	inFile  = argv[1]
	outFile = argv[2]
	# pilimage = PILImage.open(inFile)
	# npimage = np.asarray(pilimage)
	# print npimage.dtype

	ExtractROI = otbApplication.Registry.CreateApplication("ExtractROI")
	ExtractROI.SetParameterString("in", inFile)
	ExtractROI.SetParameterInt("startx", 10)
	ExtractROI.SetParameterInt("starty", 10)
	ExtractROI.SetParameterInt("sizex", 250)
	ExtractROI.SetParameterInt("sizey", 250)

	#Bug or Design ?.
	#Anyway below two is needed only for ExtractROI application
	ExtractROI.SetParameterUserValue("sizex", True)
	ExtractROI.SetParameterUserValue("sizey", True)
	ExtractROI.Execute()

	ExtractROIOut = ExtractROI.GetVectorImageAsNumpyArray("out", 'float')

	#write RGB image to file via python
	#misc.imsave('ExtractROIOut.jpg', ExtractROIOut)

	Rescale = otbApplication.Registry.CreateApplication("Rescale")
	#take numpy array from ExtractROI and feed into Rescale
	Rescale.SetVectorImageFromNumpyArray("in", ExtractROIOut)
	Rescale.SetParameterFloat("outmin", 50, True)
	Rescale.SetParameterFloat("outmax", 100, True)
	Rescale.Execute()

	RescaleOut = Rescale.GetVectorImageAsNumpyArray("out", 'float')
	# misc.imsave('RescaleOut.jpg', RescaleOut)

	DynamicConvert = otbApplication.Registry.CreateApplication("DynamicConvert")
	# take numpy output from Rescale application and feed into DynamicConvert
	DynamicConvert.SetVectorImageFromNumpyArray("in", RescaleOut)
	DynamicConvert.SetParameterString("out", "ConvertOut.png")
	DynamicConvert.ExecuteAndWriteOutput()
	DynamicConvertOut = DynamicConvert.GetVectorImageAsNumpyArray("out", 'float')

	Smoothing = otbApplication.Registry.CreateApplication("Smoothing")
	# take numpy output from Convert application and feed into Smoothing
	Smoothing.SetVectorImageFromNumpyArray("in", DynamicConvertOut)
	Smoothing.SetParameterString("type", 'anidif')
	Smoothing.SetParameterString("out", outFile)
	Smoothing.ExecuteAndWriteOutput()
