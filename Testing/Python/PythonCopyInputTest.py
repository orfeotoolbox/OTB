# -*- coding: utf-8 -*-

#
#  Example on the use of the Rescale 
#
from sys import argv
import otbApplication as otb

print "Available applications : " + str(otb.Registry.GetAvailableApplications())

app = otb.Registry.CreateApplication("CopyInput")
print app.GetParametersKeys()

app.SetParameterString("in", argv[1])
app.SetParameterString("out", argv[2])
app.Execute()
  
print dir(app)
