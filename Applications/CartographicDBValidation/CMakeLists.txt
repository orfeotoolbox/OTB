cmake_minimum_required(VERSION 2.8)

if(COMMAND CMAKE_POLICY)
 cmake_policy(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)

project( ComputePolylineFeatureFromImage )

find_package(OTB REQUIRED)
include(${OTB_USE_FILE})

OTB_CREATE_APPLICATION(NAME           ComputePolylineFeatureFromImage
                       SOURCES        otbComputePolylineFeatureFromImage.cxx
                       LINK_LIBRARIES OTBBasicFilters OTBLearning)

