
FILE(GLOB srcs "*.cxx")
SET( WrappersQtWidget_MOC_HDR otbWrapperQtWidgetEmptyParameter.h )   
QT4_WRAP_CPP(WrappersQtWidget_MOC_SRC ${WrappersQtWidget_MOC_HDR})

ADD_LIBRARY(OTBWrapperQtWidget ${srcs} ${WrappersQtWidget_MOC_SRC})
TARGET_LINK_LIBRARIES(OTBWrapperQtWidget OTBCommon OTBIO OTBWrapperCore ${QT_LIBRARIES})
