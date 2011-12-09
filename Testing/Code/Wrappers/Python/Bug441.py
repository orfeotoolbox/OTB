# Import the otb applications package
import otbApplication
import sys

# The following line creates an instance of the StereoSensorModelToElevationMap application
StereoSensorModelToElevationMap = otbApplication.Registry.CreateApplication("StereoSensorModelToElevationMap")

# The following lines set all the application parameters:
StereoSensorModelToElevationMap.SetParameterString("ref", sys.argv[1])

StereoSensorModelToElevationMap.SetParameterString("sec", sys.argv[2])

StereoSensorModelToElevationMap.SetParameterString("out", sys.argv[3])

StereoSensorModelToElevationMap.SetParameterString("dem", "DEM")

StereoSensorModelToElevationMap.SetParameterString("geoid", "egm96.grd")

# The following line execute the application
sys.exit(StereoSensorModelToElevationMap.ExecuteAndWriteOutput())
