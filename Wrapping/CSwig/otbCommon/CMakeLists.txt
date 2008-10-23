# create a list of cable config files for wrapping
SET(WRAP_SOURCES
#  wrap_itkImageSource
  wrap_otbImage
  wrap_otbVectorImage
)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(MASTER_INDEX_FILES 
		       "${WrapOTB_BINARY_DIR}/otbCommon/OTBCommon.mdx"
		       "${WrapOTB_BINARY_DIR}/CommonA/ITKCommonA.mdx"
		       "${WrapOTB_BINARY_DIR}/CommonB/ITKCommonB.mdx"
)

ITK_WRAP_LIBRARY("${WRAP_SOURCES}" OTBCommon otbCommon "ITKCommonA" "" "OTBIO;OTBCommon")

