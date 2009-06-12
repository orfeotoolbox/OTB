PROJECT(AlosModel)

cmake_minimum_required(VERSION 2.4)

FIND_PACKAGE(OTB)
IF(OTB_FOUND)
  INCLUDE(${OTB_USE_FILE})
ELSE(OTB_FOUND)
  MESSAGE(FATAL_ERROR
          "OTB not found. Please set OTB_DIR")
ENDIF(OTB_FOUND)

# SET(CMAKE_INCLUDE_CURRENT_DIR ON)


FILE(GLOB_RECURSE ossim_src "*.cpp")
INCLUDE_DIRECTORIES(${AlosModel_SOURCE_DIR})
INCLUDE_DIRECTORIES(${AlosModel_SOURCE_DIR}/otb)

ADD_EXECUTABLE(testSensorModel testSensorModel.cxx ${ossim_src})
TARGET_LINK_LIBRARIES(testSensorModel OTBCommon OTBIO)

ADD_EXECUTABLE(testAlosPalsarLeader testAlosPalsarLeader.cxx ${ossim_src})
TARGET_LINK_LIBRARIES(testAlosPalsarLeader OTBCommon OTBIO)
