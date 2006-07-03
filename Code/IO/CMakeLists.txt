# Sources of non-templated classes.

FILE(GLOB OTBIO_SRCS "*.cxx" )

ADD_LIBRARY(OTBIO ${OTBIO_SRCS})
TARGET_LINK_LIBRARIES (OTBIO gdal OTBCommon ITKIO ITKCommon)

INSTALL_TARGETS(/lib/otb OTBIO )
INSTALL_FILES(/include/otb/IO "(\\.h|\\.txx)$")
