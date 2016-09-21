# -*- coding: utf-8 -*-
# Author: Rashad Kanavath <rashad.kanavath@c-s.fr>
#
# Copyright: (c) CS Systemes d'information. All rights reserved
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

	Convert = otbApplication.Registry.CreateApplication("Convert")
	# take numpy output from Rescale application and feed into Convert
	Convert.SetVectorImageFromNumpyArray("in", RescaleOut)
	Convert.SetParameterString("out", "ConvertOut.png")
	Convert.ExecuteAndWriteOutput()
	ConvertOut = Convert.GetVectorImageAsNumpyArray("out", 'float')

	Smoothing = otbApplication.Registry.CreateApplication("Smoothing")
	# take numpy output from Convert application and feed into Smoothing
	Smoothing.SetVectorImageFromNumpyArray("in", ConvertOut)
	Smoothing.SetParameterString("type", 'anidif')
	Smoothing.SetParameterString("out", outFile)
	Smoothing.ExecuteAndWriteOutput()
