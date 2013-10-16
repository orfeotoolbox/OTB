# -*- coding: utf-8 -*-

#
#  Example on the use of the outxml via BandMath 
#
from sys import argv
import otbApplication as otb

imagelist = argv[1:-2]
exp = "cos(im1b1)+im2b1*im3b1-im3b2+ndvi(im3b3, im3b4)"
out = argv[-2]
outxml = argv[-1]
app = otb.Registry_CreateApplication('BandMath')
app.SetParameterStringList("il", imagelist)
app.SetParameterString("out", out)
app.SetParameterString("exp", exp)
app.SetParameterString("outxml", outxml)
app.UpdateParameters()
for paramKey in app.GetParametersKeys():
  #app.GetParameterValue(paramKey)
  print app.HasUserValue(paramKey)
app.ExecuteAndWriteOutput()


