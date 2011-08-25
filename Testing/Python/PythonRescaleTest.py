# -*- coding: utf-8 -*-

#
#  Example on the use of the Rescale 
#
from sys import argv
import otbApplication as otb

print "Available applications : " + str(otb.Registry.GetAvailableApplications())

app = otb.Registry.CreateApplication("Rescale")
print app.GetParametersKeys()

app.SetParameterString("in", argv[1])

app.SetParameterFloat("outmin", 1)
app.SetParameterFloat("outmax", 100)
app.SetParameterString("out", argv[2] + ".tif")
app.ExecuteAndWriteOutput()
  
print dir(app)
