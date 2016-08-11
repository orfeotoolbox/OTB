# -*- coding: utf-8 -*-

#
#  Example on the use of the Rescale 
#
from sys import argv
import otbApplication as otb

app1 = otb.Registry.CreateApplication("Smoothing")
app2 = otb.Registry.CreateApplication("Smoothing")
app3 = otb.Registry.CreateApplication("Smoothing")
app4 = otb.Registry.CreateApplication("ConcatenateImages")

app1.IN = argv[1]
app1.Execute()

app2.SetParameterInputImage("in",app1.GetParameterOutputImage("out"))
app2.Execute()

app3.IN = argv[1]
app3.Execute()

app4.AddImageToParameterInputImageList("il",app2.GetParameterOutputImage("out"));
app4.AddImageToParameterInputImageList("il",app3.GetParameterOutputImage("out"));

app4.OUT = argv[2]
app4.ExecuteAndWriteOutput()
