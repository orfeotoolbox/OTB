# -*- coding: utf-8 -*-

#
#  Example on the use of the inxml via BandMath 
#
from sys import argv
import otbApplication as otb

app = otb.Registry_CreateApplication('BandMath')
app.SetParameterString("inxml", argv[1])
app.UpdateParameters()
imagelist = argv[2:-1]
app.SetParameterStringList("il", imagelist)
app.SetParameterString("out"  , argv[-1])
app.ExecuteAndWriteOutput()



