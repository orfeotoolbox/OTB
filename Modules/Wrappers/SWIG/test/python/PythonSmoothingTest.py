# -*- coding: utf-8 -*-

#
#  Example on the use of the Smoothing
#

def test(otb, argv):
	print "Available applications : " + str(otb.Registry.GetAvailableApplications())

	app = otb.Registry.CreateApplication("Smoothing")
	print app.GetParametersKeys()

	app.SetParameterString("in", argv[1])

	for type in ['mean', 'gaussian', 'anidif']:
		print 'Testing ' + type
		app.SetParameterString("type", type)
		app.SetParameterString("out", argv[2] + type + ".tif")
		app.ExecuteAndWriteOutput()

		print dir(app)
