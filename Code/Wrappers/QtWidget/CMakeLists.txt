include_directories(${QT_INCLUDE_DIRS})
file(GLOB srcs "*.cxx")

set( WrappersQtWidget_MOC_HDR
       otbWrapperQtWidgetEmptyParameter.h
       otbWrapperQtWidgetFloatParameter.h
       otbWrapperQtWidgetIntParameter.h
       otbWrapperQtWidgetStringParameter.h
       otbWrapperQtWidgetStringListParameter.h
       otbWrapperQtWidgetChoiceParameter.h
       otbWrapperQtWidgetInputImageParameter.h
       otbWrapperQtWidgetComplexInputImageParameter.h
       otbWrapperQtWidgetComplexOutputImageParameter.h
       otbWrapperQtWidgetInputImageListParameter.h
       otbWrapperQtWidgetInputVectorDataListParameter.h
       otbWrapperQtWidgetInputVectorDataParameter.h
       otbWrapperQtWidgetOutputImageParameter.h
       otbWrapperQtWidgetOutputVectorDataParameter.h
       otbWrapperQtWidgetOutputProcessXMLParameter.h
       otbWrapperQtWidgetInputProcessXMLParameter.h
       otbWrapperQtWidgetParameterGroup.h
       otbWrapperQtWidgetParameterLabel.h
       otbWrapperQtWidgetParameterBase.h
       otbWrapperQtWidgetModel.h
       otbWrapperQtWidgetView.h
       otbWrapperQtWidgetProgressReport.h
       otbWrapperQtWidgetListViewParameter.h
       otbQtFileSelectionWidget.h
       otbQtStringSelectionWidget.h
       otbQtApplication.h
       itkQtProgressBar.h
       otbWrapperQtWidgetInputFilenameParameter.h
       otbWrapperQtWidgetInputFilenameListParameter.h
       otbWrapperQtWidgetOutputFilenameParameter.h
       otbQtLogOutput.h
       otbWrapperQtWidgetDirectoryParameter.h
       otbWrapperQtWidgetSimpleProgressReport.h
       otbWrapperQtWidgetRAMParameter.h
      )

QT4_WRAP_CPP(WrappersQtWidget_MOC_SRC ${WrappersQtWidget_MOC_HDR})

add_library(OTBWrapperQtWidget ${srcs} ${WrappersQtWidget_MOC_SRC})
target_link_libraries(OTBWrapperQtWidget OTBApplicationEngine ${QT_LIBRARIES})


if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBWrapperQtWidget PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif()

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBWrapperQtWidget
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif()


if(NOT OTB_INSTALL_NO_DEVELOPMENT)

  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*h")
  install(FILES ${__files1} ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Wrappers/QtWidget
    COMPONENT Development)
endif()
