# -*- coding: utf-8 -*-

#
#  Example on the use of the Rescale 
#
from sys import argv
import otbApplication as otb

app1 = otb.Registry.CreateApplication("Smoothing")
app2 = otb.Registry.CreateApplication("Smoothing")

app1.IN = argv[1]
app1.Execute()

app2.SetParameterInputImage("in",app1.GetParameterOutputImage("out"))

app2.OUT = argv[2]
app2.ExecuteAndWriteOutput()
