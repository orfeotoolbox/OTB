# -*- coding: utf-8 -*-

#
#  Example on the use of the Smoothing 
#
from sys import argv
import otbApplication as otb

print "Available applications : " + str(otb.Registry.GetAvailableApplications())

app = otb.Registry.CreateApplication("Addition")
print app.GetParametersKeys()

app.SetParameterFloat("a", float(argv[1]))
app.SetParameterFloat("b", float(argv[2]))

app.Execute()
  
print dir(app)
