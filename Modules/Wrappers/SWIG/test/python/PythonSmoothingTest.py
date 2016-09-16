# -*- coding: utf-8 -*-

#
#  Example on the use of the Smoothing
#

def test(otb, argv):
	print( "Available applications : " + str(otb.Registry.GetAvailableApplications()) )

	app = otb.Registry.CreateApplication("Smoothing")
	print( app.GetParametersKeys() )

	app.SetParameterString("in", argv[1])

	for type_ in ['mean', 'gaussian', 'anidif']:
		print( 'Testing ' + type_ )
		app.SetParameterString("type", type_)
		app.SetParameterString("out", argv[2] + type_ + ".tif")
		app.ExecuteAndWriteOutput()

		print( dir(app) )
