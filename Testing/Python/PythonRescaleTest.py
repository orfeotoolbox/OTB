# -*- coding: utf-8 -*-

#
#  Example on the use of the Rescale 
#
from sys import argv
import PythonApplication
#import otbApplication as otb

app = PythonApplication.PythonApplicationClass("Rescale")

print app.m_App.GetParametersKeys()
app.SetParameterString("in", argv[1])


app.SetParameterString("in", argv[1])

app.SetParameterFloat("outmin", 1)
app.SetParameterFloat("outmax", 100)
app.SetParameterString("out", argv[2] + ".tif")
app.ExecuteAndWriteOutput()
  
print dir(app)
