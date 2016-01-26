set(OTBQtWidget_SRC
  otbQtLogOutput.cxx
  otbWrapperQtWidgetModel.cxx
  otbWrapperQtWidgetParameterLabel.cxx
  otbWrapperQtWidgetView.cxx
  otbWrapperQtWidgetInputProcessXMLParameter.cxx
  otbWrapperQtWidgetIntParameter.cxx
  otbWrapperQtWidgetInputVectorDataListParameter.cxx
  otbWrapperQtWidgetSimpleProgressReport.cxx
  otbWrapperQtWidgetFloatParameter.cxx
  otbWrapperQtWidgetChoiceParameter.cxx
  otbWrapperQtWidgetComplexOutputImageParameter.cxx
  otbWrapperQtWidgetOutputFilenameParameter.cxx
  otbQtStringSelectionWidget.cxx
  itkQtProgressBar.cxx
  otbWrapperQtWidgetStringParameter.cxx
  otbQtApplication.cxx
  otbWrapperQtWidgetParameterFactory.cxx
  otbWrapperQtWidgetListViewParameter.cxx
  otbWrapperQtWidgetDirectoryParameter.cxx
  otbWrapperQtWidgetOutputImageParameter.cxx
  otbWrapperQtWidgetInputVectorDataParameter.cxx
  otbQtFileSelectionWidget.cxx
  otbWrapperQtWidgetParameterGroup.cxx
  otbWrapperQtWidgetInputFilenameListParameter.cxx
  otbWrapperQtWidgetComplexInputImageParameter.cxx
  otbWrapperQtWidgetRAMParameter.cxx
  otbWrapperQtWidgetProgressReport.cxx
  otbWrapperQtWidgetOutputProcessXMLParameter.cxx
  otbWrapperQtWidgetParameterBase.cxx
  otbWrapperQtWidgetInputImageParameter.cxx
  otbWrapperQtWidgetStringListParameter.cxx
  otbWrapperQtWidgetEmptyParameter.cxx
  otbWrapperQtWidgetOutputVectorDataParameter.cxx
  otbWrapperQtWidgetInputFilenameParameter.cxx
  otbWrapperQtWidgetInputImageListParameter.cxx
  )

set(OTBQtWidget_MOC_HDR
  ../include/otbWrapperQtWidgetEmptyParameter.h
  ../include/otbWrapperQtWidgetFloatParameter.h
  ../include/otbWrapperQtWidgetIntParameter.h
  ../include/otbWrapperQtWidgetStringParameter.h
  ../include/otbWrapperQtWidgetStringListParameter.h
  ../include/otbWrapperQtWidgetChoiceParameter.h
  ../include/otbWrapperQtWidgetInputImageParameter.h
  ../include/otbWrapperQtWidgetComplexInputImageParameter.h
  ../include/otbWrapperQtWidgetComplexOutputImageParameter.h
  ../include/otbWrapperQtWidgetInputImageListParameter.h
  ../include/otbWrapperQtWidgetInputVectorDataListParameter.h
  ../include/otbWrapperQtWidgetInputVectorDataParameter.h
  ../include/otbWrapperQtWidgetOutputImageParameter.h
  ../include/otbWrapperQtWidgetOutputVectorDataParameter.h
  ../include/otbWrapperQtWidgetOutputProcessXMLParameter.h
  ../include/otbWrapperQtWidgetInputProcessXMLParameter.h
  ../include/otbWrapperQtWidgetParameterGroup.h
  ../include/otbWrapperQtWidgetParameterLabel.h
  ../include/otbWrapperQtWidgetParameterBase.h
  ../include/otbWrapperQtWidgetModel.h
  ../include/otbWrapperQtWidgetView.h
  ../include/otbWrapperQtWidgetProgressReport.h
  ../include/otbWrapperQtWidgetListViewParameter.h
  ../include/otbQtFileSelectionWidget.h
  ../include/otbQtStringSelectionWidget.h
  ../include/otbQtApplication.h
  ../include/itkQtProgressBar.h
  ../include/otbWrapperQtWidgetInputFilenameParameter.h
  ../include/otbWrapperQtWidgetInputFilenameListParameter.h
  ../include/otbWrapperQtWidgetOutputFilenameParameter.h
  ../include/otbQtLogOutput.h
  ../include/otbWrapperQtWidgetDirectoryParameter.h
  ../include/otbWrapperQtWidgetSimpleProgressReport.h
  ../include/otbWrapperQtWidgetRAMParameter.h
  )

QT4_WRAP_CPP(OTBQtWidget_MOC_SRC ${OTBQtWidget_MOC_HDR})

add_library(OTBQtWidget ${OTBQtWidget_SRC} ${OTBQtWidget_MOC_SRC})
target_link_libraries(OTBQtWidget
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBQt4_LIBRARIES}
  )
otb_module_target(OTBQtWidget)

add_executable(otbApplicationLauncherQt otbApplicationLauncherQt.cxx)
target_link_libraries(otbApplicationLauncherQt
  ${OTBQtWidget_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBQt4_LIBRARIES}
  )
otb_module_target(otbApplicationLauncherQt)

# Where we will install the script in the build tree
get_target_property(GUI_OUTPUT_DIR otbApplicationLauncherQt RUNTIME_OUTPUT_DIRECTORY)

# Generate a script in the build dir, next to the gui launcher
# Need a two-step process since configure_file don't support permissions
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts)


if (WIN32)
configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbgui.bat.in
                ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/otbgui.bat
                @ONLY )
file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/otbgui.bat
     DESTINATION ${GUI_OUTPUT_DIR}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# TODO : test if NO_INSTALL is set
install(PROGRAMS ${GUI_OUTPUT_DIR}/otbgui.bat
        DESTINATION ${OTBQtWidget_INSTALL_RUNTIME_DIR}
        COMPONENT Runtime)

else()
configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbgui.sh.in
                ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/otbgui
                @ONLY )
file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/otbgui
     DESTINATION ${GUI_OUTPUT_DIR}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# TODO : test if NO_INSTALL is set
install(PROGRAMS ${GUI_OUTPUT_DIR}/otbgui
        DESTINATION ${OTBQtWidget_INSTALL_RUNTIME_DIR}
        COMPONENT Runtime)

endif()
