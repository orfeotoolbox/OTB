# Sources of non-templated classes.

FILE(GLOB OTBIO_SRCS "*.cxx" )
#SET(OTBCommon_SRCS
#  otbExceptionObject.cxx
#)

ADD_LIBRARY(OTBIO ${OTBIO_SRCS})
TARGET_LINK_LIBRARIES (OTBIO ITKIO)

INSTALL_TARGETS(/lib/otb OTBIO )
INSTALL_FILES(/include/otb/IO "(\\.h|\\.txx)$")
