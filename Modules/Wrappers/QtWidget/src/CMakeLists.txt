#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set(OTBQtWidget_SRC
  otbQtLogOutput.cxx
  otbWrapperQtWidgetModel.cxx
  otbWrapperQtWidgetParameterLabel.cxx
  otbWrapperQtWidgetView.cxx
  otbWrapperQtWidgetMainWindow.cxx
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
  otbWrapperQtWidgetOutputVectorDataParameter.cxx
  otbWrapperQtWidgetInputFilenameParameter.cxx
  otbWrapperQtWidgetInputImageListParameter.cxx
  otbWrapperQtWidgetListEditWidget.cxx
  otbWrapperQtWidgetListEditItemModel.cxx
  otbWrapperQtWidgetParameterList.cxx
  otbWrapperQtWidgetBoolParameter.cxx
  otbWrapperQtWidgetSpinBoxes.cxx
  )

set(OTBQtWidget_MOC_HDR
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
  ../include/otbWrapperQtWidgetMainWindow.h
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
  ../include/otbWrapperQtWidgetListEditWidget.h
  ../include/otbWrapperQtWidgetListEditItemModel.h
  ../include/otbWrapperQtWidgetParameterList.h
  ../include/otbWrapperQtWidgetBoolParameter.h
  ../include/otbWrapperQtWidgetSpinBoxes.h
  )

set( OTBQtWidget_FORMS
  otbWrapperQtWidgetListEditWidget.ui
  appmainwindow.ui
  )

set( OTBQtWidget_RESOURCES
  otbWrapperQtWidgetIcons.qrc
  )

qt5_wrap_cpp( OTBQtWidget_MOC_SRC ${OTBQtWidget_MOC_HDR} OPTIONS -p "." )
qt5_wrap_ui( OTBQtWidget_FORMS_HEADERS ${OTBQtWidget_FORMS} )
qt5_add_resources( OTBQtWidget_RESOURCES_RCC ${OTBQtWidget_RESOURCES} OPTIONS "-no-compress")

add_library( OTBQtWidget
  ${OTBQtWidget_SRC}
  ${OTBQtWidget_FORMS_HEADERS}
  ${OTBQtWidget_MOC_SRC}
  ${OTBQtWidget_RESOURCES_RCC}
  )

target_link_libraries( OTBQtWidget
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBQt_LIBRARIES}
  ${OTBQtAdapters_LIBRARIES}
  )

otb_module_target(OTBQtWidget)

add_executable(otbApplicationLauncherQt otbApplicationLauncherQt.cxx)
target_link_libraries(otbApplicationLauncherQt
  ${OTBQtWidget_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBQt_LIBRARIES}
  )
otb_module_target(otbApplicationLauncherQt)
set_linker_stack_size_flag(otbApplicationLauncherQt 10000000)

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
