# -*- coding: utf-8 -*-

#
#  Example on the use of the outxml via BandMath 
#
from sys import argv
import otbApplication as otb

filelen = len(argv) - 2
last = len(argv) - 1
imagelist = []
for i in range(1,filelen ):
  imagelist.append(argv[i])

exp = "cos(im1b1)+im2b1*im3b1-im3b2+ndvi(im3b3, im3b4)"
out = argv[last-1]
outxml = argv[last]
app = otb.Registry_CreateApplication('BandMath')
app.SetParameterStringList("il", imagelist)
app.SetParameterString("out", out)
app.SetParameterString("exp", exp)
app.SetParameterString("outxml",outxml)

for paramKey in app.GetParametersKeys():
  app.GetParameterValue(paramKey)
app.ExecuteAndWriteOutput()


