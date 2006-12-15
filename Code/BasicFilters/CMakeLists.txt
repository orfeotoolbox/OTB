
# Sources of non-templated classes.
FILE(GLOB OTBBasicFilters_SRCS "*.cxx" )

ADD_LIBRARY(OTBBasicFilters ${OTBBasicFilters_SRCS})
TARGET_LINK_LIBRARIES (OTBBasicFilters ITKCommon)

INSTALL_TARGETS(/lib/otb OTBBasicFilters )
INSTALL_FILES(/include/otb/BasicFilters "(\\.h|\\.txx)$")
