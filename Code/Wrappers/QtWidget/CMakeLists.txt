include_directories(${QT_INCLUDE_DIRS})
file(GLOB srcs "*.cxx")

set( WrappersQtWidget_MOC_HDR
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
       otbWrapperQtWidgetProgressReport.h
       itkQtProgressBar.h
       )

QT4_WRAP_CPP(WrappersQtWidget_MOC_SRC ${WrappersQtWidget_MOC_HDR})

add_library(OTBWrapperQtWidget ${srcs} ${WrappersQtWidget_MOC_SRC})
target_link_libraries(OTBWrapperQtWidget OTBApplicationEngine ${QT_LIBRARIES})


IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(OTBWrapperCommandLine PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)


install(TARGETS OTBWrapperQtWidget
        RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR_CM24}
        LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24}
        ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24})
