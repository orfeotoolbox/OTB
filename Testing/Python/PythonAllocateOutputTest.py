# -*- coding: utf-8 -*-

#
#  Example on the use of the Rescale 
#
from sys import argv
import otbApplication as otb

print "Available applications : " + str(otb.Registry.GetAvailableApplications())

app = otb.Registry.CreateApplication("AllocateOutput")
print app.GetParametersKeys()

app.SetParameterString("out", argv[1])
app.Execute()
  
print dir(app)
