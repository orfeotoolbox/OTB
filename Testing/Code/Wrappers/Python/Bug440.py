# Import the otb applications package
import otbApplication
import sys

# The following line creates an instance of the Convert application 
Convert = otbApplication.Registry.CreateApplication("Convert")

# The following lines set all the application parameters:
Convert.SetParameterString("in", sys.argv[1])

Convert.SetParameterString("out", sys.argv[2])

Convert.SetParameterString("type","linear")

# The following line execute the application
sys.exit(Convert.ExecuteAndWriteOutput())
