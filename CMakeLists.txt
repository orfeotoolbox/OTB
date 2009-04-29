
cmake_minimum_required(VERSION 2.6)

PROJECT(mapnik)

FIND_PACKAGE(OTB)
IF(OTB_FOUND)
  INCLUDE(${OTB_USE_FILE})
ELSE(OTB_FOUND)
  MESSAGE(FATAL_ERROR
          "OTB not found. Please set OTB_DIR")
ENDIF(OTB_FOUND)


INCLUDE_DIRECTORIES("/home/christop/slash/include" )
INCLUDE_DIRECTORIES("/usr/include/freetype2" )
LINK_DIRECTORIES("/home/christop/slash/lib")



ADD_EXECUTABLE(myOwnMapnik myOwnMapnik.cxx)
TARGET_LINK_LIBRARIES(myOwnMapnik mapnik)

ADD_EXECUTABLE(mapnikFromVectorData mapnikFromVectorData.cxx)
TARGET_LINK_LIBRARIES(mapnikFromVectorData OTBCommon OTBIO mapnik)

ADD_EXECUTABLE(mapnikOTBClasses mapnikOTBClasses.cxx)
TARGET_LINK_LIBRARIES(mapnikOTBClasses OTBCommon OTBIO mapnik)
