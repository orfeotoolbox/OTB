# Sources of non-templated classes.

FILE(GLOB OTBVisu_SRCS "*.cxx" )

ADD_LIBRARY(OTBVisu ${OTBVisu_SRCS})
TARGET_LINK_LIBRARIES (OTBVisu OTBGui ITKCommon OTBCommon ${FLTK_LIBRARIES} ${OPENGL_LIBRARIES})

INSTALL_TARGETS(/lib/otb OTBVisu )
INSTALL_FILES(/include/otb/Visu "(\\.h|\\.txx)$")
