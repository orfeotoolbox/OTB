

def test(otbApplication, argv):
	RadiometricIndices = otbApplication.Registry.CreateApplication('RadiometricIndices')
	RadiometricIndices.SetParameterString("in", argv[1])
	RadiometricIndices.SetParameterString("out", argv[2])
	RadiometricIndices.SetParameterInt("channels.red", 3)
	RadiometricIndices.SetParameterInt("channels.green", 2)
	RadiometricIndices.SetParameterInt("channels.nir", 4)

	RadiometricIndices.SetParameterStringList("list",	["Vegetation:NDVI","Vegetation:RVI","Vegetation:IPVI"])

	print( RadiometricIndices.GetParameterStringList("list") )

	print( RadiometricIndices.GetParameterValue("list") )

	RadiometricIndices.ExecuteAndWriteOutput()
