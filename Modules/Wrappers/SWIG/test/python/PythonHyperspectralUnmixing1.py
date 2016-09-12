# -*- coding: utf-8 -*-

#
#  Example on the use of the Rescale
#

def test(otb, argv):
	app = otb.Registry.CreateApplication("HyperspectralUnmixing")

	# input image
	app.SetParameterString("in", argv[1])

	#output image
	app.SetParameterString("out", argv[2])

	#input endmembers
	app.SetParameterString("ie", argv[3])

	#unmixing algorithm
	app.SetParameterString("ua", argv[4])

	app.ExecuteAndWriteOutput()
