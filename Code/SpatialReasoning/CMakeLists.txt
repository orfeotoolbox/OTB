# Sources of non-templated classes.

FILE(GLOB OTBSpatialReasoning_SRCS "*.cxx" )

ADD_LIBRARY(OTBSpatialReasoning ${OTBSpatialReasoning_SRCS})
TARGET_LINK_LIBRARIES (OTBSpatialReasoning ITKCommon)

INSTALL_TARGETS(/lib/otb OTBSpatialReasoning )
INSTALL_FILES(/include/otb/SpatialReasoning "(\\.h|\\.txx)$")
