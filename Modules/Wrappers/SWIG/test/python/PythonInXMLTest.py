# -*- coding: utf-8 -*-

#
#  Example on the use of the inxml via BandMath
#

def test(otb, argv):
	app = otb.Registry.CreateApplication('BandMath')

	imagelist = argv[2:-1]
	app.SetParameterStringList("il", imagelist, True)

	app.SetParameterString("inxml", argv[1])
	app.UpdateParameters()

	app.SetParameterString("out"  , argv[-1])
	app.ExecuteAndWriteOutput()
