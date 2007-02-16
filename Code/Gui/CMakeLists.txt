# Sources of non-templated classes.

FILE(GLOB OTBGui_SRCS "*.cxx" )

ADD_LIBRARY(OTBGui ${OTBGui_SRCS})
TARGET_LINK_LIBRARIES (OTBGui ITKCommon)

INSTALL_TARGETS(/lib/otb OTBGui )
INSTALL_FILES(/include/otb/Gui "(\\.h|\\.txx)$")
