# Sources of non-templated classes.

FILE(GLOB OTBMultiScale_SRCS "*.cxx" )

ADD_LIBRARY(OTBMultiScale ${OTBMultiScale_SRCS})
TARGET_LINK_LIBRARIES (OTBMultiScale ITKCommon)

INSTALL_TARGETS(/lib/otb OTBMultiScale )
INSTALL_FILES(/include/otb/MultiScale "(\\.h|\\.txx)$")
