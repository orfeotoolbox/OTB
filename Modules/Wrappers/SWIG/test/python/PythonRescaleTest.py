# -*- coding: utf-8 -*-

#
#  Example on the use of the Rescale
#
def test(otb, argv):
	app = otb.Registry.CreateApplication("Rescale")
	app.SetParameterString("in", argv[1])
	app.SetParameterFloat("outmin", 1)
	app.SetParameterFloat("outmax", 100)
	app.SetParameterString("out", argv[2] + ".tif")
	app.ExecuteAndWriteOutput()
