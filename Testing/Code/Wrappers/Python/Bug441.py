#!/usr/bin/python

# Import the otb applications package
import otbApplication, sys

# The following line creates an instance of the StereoSensorModelToElevationMap application 
StereoSensorModelToElevationMap = otbApplication.Registry.CreateApplication("StereoSensorModelToElevationMap")

# The following lines set all the application parameters:
StereoSensorModelToElevationMap.SetParameterString("ref", sys.argv[1])
StereoSensorModelToElevationMap.SetParameterString("sec", sys.argv[2])
StereoSensorModelToElevationMap.SetParameterString("out", sys.argv[3])
StereoSensorModelToElevationMap.SetParameterString("elev.dem",  "dem_wrong_path")
StereoSensorModelToElevationMap.SetParameterString("elev.geoid", "geoid_wrong_file")

# This bug verifies that a proper exception is raised during ExecuteAndWriteOutput
try:
  StereoSensorModelToElevationMap.ExecuteAndWriteOutput()
except:
  print "Catching exception : this is the expected behavior - TEST PASS"
  sys.exit( 0 )

print "Should have raised an exception, but did not - TEST FAIL"
sys.exit( 1 )

