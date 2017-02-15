# Import the otb applications package

import sys
def test(otbApplication, argv):
	# The following line creates an instance of the Convert application
	Convert = otbApplication.Registry.CreateApplication("Convert")

	# The following lines set all the application parameters:
	Convert.SetParameterString("in", argv[1])

	Convert.SetParameterString("out", argv[2])

	Convert.SetParameterString("type","linear")

	# The following line execute the application
	sys.exit(Convert.ExecuteAndWriteOutput())
