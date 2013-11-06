import otbApplication
import sys

RadiometricIndices = otbApplication.Registry.CreateApplication('RadiometricIndices')
RadiometricIndices.SetParameterString("in", sys.argv[1])
RadiometricIndices.SetParameterString("out", sys.argv[2])
RadiometricIndices.SetParameterInt("channels.red", 3)
RadiometricIndices.SetParameterInt("channels.green", 2)
RadiometricIndices.SetParameterInt("channels.nir", 4)

RadiometricIndices.SetParameterStringList("list", 
        ["Vegetation:NDVI","Vegetation:RVI","Vegetation:IPVI"])
        
print RadiometricIndices.GetParameterStringList("list")

RadiometricIndices.ExecuteAndWriteOutput()