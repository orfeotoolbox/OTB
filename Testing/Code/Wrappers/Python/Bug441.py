#!/usr/bin/python

# Import the otb applications package
import otbApplication, sys

# The following line creates an instance of the StereoSensorModelToElevationMap application 
StereoSensorModelToElevationMap = otbApplication.Registry.CreateApplication("StereoSensorModelToElevationMap")

# Needed because of bug 440
StereoSensorModelToElevationMap.Init()

# The following lines set all the application parameters:
StereoSensorModelToElevationMap.SetParameterString("ref", sys.argv[1])

StereoSensorModelToElevationMap.SetParameterString("sec", sys.argv[2])

StereoSensorModelToElevationMap.SetParameterString("out", sys.argv[3])

StereoSensorModelToElevationMap.SetParameterString("dem", "DEM")

# The following line execute the application
StereoSensorModelToElevationMap.ExecuteAndWriteOutput()
