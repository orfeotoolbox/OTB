# -*- coding: utf-8 -*-

#
#  Example on the use of the Rescale 
#
from sys import argv
import otbApplication as otb

app = otb.Registry.CreateApplication("OrthoRectification")

app.SetParameterString("in", argv[1])
app.SetParameterString("out", argv[2] + ".tif")
app.SetParameterFloat("outputs.ulx",float(argv[3]))
app.SetParameterFloat("outputs.uly",float(argv[4]))
app.SetParameterInt("outputs.sizex",int(argv[5]))
app.SetParameterInt("outputs.sizey",int(argv[6]))
app.SetParameterFloat("outputs.spacingx",float(argv[7]))
app.SetParameterFloat("outputs.spacingy",float(argv[8]))

app.SetParameterString("map", argv[9])
app.SetParameterString("dem", argv[10])

app.ExecuteAndWriteOutput()

print dir(app)

# print some values
print app.GetParameterRole()
print app.GetDocName()
print app.GetDocLongDescription()
print app.DocCLExample()
print app.DocAuthors()
print app.DocLimitations()
print app.DocSeeAlso()

print app.GetParameterOutputImagePixelType("out")
print app.GetParameterOutputImagePixelType("int") # dummy
