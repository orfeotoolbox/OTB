# -*- coding: utf-8 -*-

#
#  Example on the use of the outxml via BandMath 
#
import sys
import otbApplication as otb

filelen = sys.argc-2
imagelist = []
for i in range(1,filelen):
  imagelist.append(sys.argv[i])


app = otb.Registry_CreateApplication('BandMath')
app.SetParameterStringList("il", imagelist)
app.SetParameterString("out", sys.argv[filelen + 1])
app.SetParameterString("exp", "cos(im1b1)+im2b1*im3b1-im3b2+ndvi(im3b3, im3b4)")
app.SetParameterString("outxml", sys.argv[filelen + 2])
#app.SetParameterString("inxml", "/home/rashad/python_xmlout.xml")

for paramKey in app.GetParametersKeys():
  app.GetParameterValue(paramKey)
#app.ExecuteAndWriteOutput()


