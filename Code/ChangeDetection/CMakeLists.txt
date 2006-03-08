# Sources of non-templated classes.

FILE(GLOB OTBChangeDetection_SRCS "*.cxx" )

ADD_LIBRARY(OTBChangeDetection ${OTBChangeDetection_SRCS})
TARGET_LINK_LIBRARIES (OTBChangeDetection OTBCommon OTBIO ITKCommon)

INSTALL_TARGETS(/lib/otb OTBChangeDetection )
INSTALL_FILES(/include/otb/ChangeDetection "(\\.h|\\.txx)$")
