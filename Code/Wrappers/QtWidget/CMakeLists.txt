INCLUDE_DIRECTORIES(${QT_INCLUDE_DIRS})
FILE(GLOB srcs "*.cxx")

SET( WrappersQtWidget_MOC_HDR
       otbWrapperQtWidgetEmptyParameter.h
       otbWrapperQtWidgetFloatParameter.h
       otbWrapperQtWidgetIntParameter.h
       otbWrapperQtWidgetStringParameter.h
       otbWrapperQtWidgetChoiceParameter.h
       otbWrapperQtWidgetInputImageParameter.h
       otbWrapperQtWidgetOutputImageParameter.h
       otbWrapperQtWidgetParameterGroup.h
       otbWrapperQtWidgetParameterLabel.h
       otbWrapperQtWidgetParameterBase.h
       otbWrapperQtWidgetModel.h      
       otbWrapperQtWidgetView.h  
       )

QT4_WRAP_CPP(WrappersQtWidget_MOC_SRC ${WrappersQtWidget_MOC_HDR})

ADD_LIBRARY(OTBWrapperQtWidget ${srcs} ${WrappersQtWidget_MOC_SRC})
TARGET_LINK_LIBRARIES(OTBWrapperQtWidget OTBCommon OTBIO OTBWrapperCore ${QT_LIBRARIES})
