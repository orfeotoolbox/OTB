#!/usr/bin/python

# Import the otb applications package
import otbApplication, sys

# The following line creates an instance of the StereoSensorModelToElevationMap application 
StereoSensorModelToElevationMap = otbApplication.Registry.CreateApplication("StereoSensorModelToElevationMap")

# The following lines set all the application parameters:
StereoSensorModelToElevationMap.SetParameterString("ref", sys.argv[1])
StereoSensorModelToElevationMap.SetParameterString("sec", sys.argv[2])
StereoSensorModelToElevationMap.SetParameterString("out", sys.argv[3])
StereoSensorModelToElevationMap.SetParameterString("elev", "dem")
StereoSensorModelToElevationMap.SetParameterString("elev.dem.path",  "dem_wrong_path")
StereoSensorModelToElevationMap.SetParameterString("elev.dem.geoid", "geoid_wrong_file")

# This bug verifies that no exception is raised during ExecuteAndWriteOutput
# It does **NOT** check that the execution of the application succeeds
try:
  StereoSensorModelToElevationMap.ExecuteAndWriteOutput()
except:
  raise

sys.exit( 0 )
