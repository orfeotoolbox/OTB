# -*- coding: utf-8 -*-

#
#  Example on the use of the inxml via BandMath
#

def test(otb, argv):
	app = otb.Registry.CreateApplication('BandMath')

	app.SetParameterString("inxml", argv[1])
	app.UpdateParameters()

	imagelist = argv[2:-1]
	app.SetParameterStringList("il", imagelist, True)

	app.SetParameterString("out"  , argv[-1])
	app.ExecuteAndWriteOutput()
