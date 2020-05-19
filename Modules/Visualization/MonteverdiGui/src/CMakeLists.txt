#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#############################################################################
set( OTBMonteverdiGUI_SRCS
  mvdAboutDialog.cxx
  mvdAbstractDragAndDropEventFilter.cxx
  mvdAbstractModelController.cxx
  mvdColorBandDynamicsWidget.cxx
  mvdColorDynamicsController.cxx
  mvdColorDynamicsWidget.cxx
  mvdColorSetupController.cxx
  mvdColorSetupWidget.cxx
  mvdDoubleValidator.cxx
  mvdDropLineEdit.cxx
  mvdFilenameDragAndDropEventFilter.cxx
  mvdGui.cxx
  mvdHistogramController.cxx
  mvdHistogramPlotPicker.cxx
  mvdHistogramWidget.cxx
  mvdI18nApplication.cxx
  mvdI18nMainWindow.cxx
  mvdImageViewRenderer.cxx
  mvdImageViewManipulator.cxx
  mvdImageViewWidget.cxx
  mvdImportImagesDialog.cxx
  mvdImportSubDatasetDialog.cxx
  mvdKeymapDialog.cxx
  mvdLayerStackController.cxx
  mvdLayerStackItemModel.cxx
  mvdLayerStackWidget.cxx
  mvdMainWindowTitleLoader.cxx
  mvdMultiResolutionPyramidWidget.cxx
  #mvdMyWidget.cxx
  mvdPixelDescriptionWidget.cxx
  mvdProjectionBarWidget.cxx
  mvdQuicklookViewManipulator.cxx
  mvdQuicklookViewRenderer.cxx
  mvdSearchableTreeWidget.cxx
  mvdShaderWidget.cxx
  mvdStatusBarWidget.cxx
  mvdTaskProgressDialog.cxx
  mvdTreeWidget.cxx
  mvdTreeWidgetItem.cxx
  mvdTreeWidgetItemDragAndDropEventFilter.cxx
 )

#############################################################################
set( OTBMonteverdiGUI_HEADERS_MOC
  ../include/mvdAboutDialog.h
  ../include/mvdAbstractDragAndDropEventFilter.h
  ../include/mvdAbstractImageViewManipulator.h
  ../include/mvdAbstractImageViewRenderer.h
  ../include/mvdAbstractModelController.h
  ../include/mvdColorBandDynamicsWidget.h
  ../include/mvdColorDynamicsController.h
  ../include/mvdColorDynamicsWidget.h
  ../include/mvdColorSetupController.h
  ../include/mvdColorSetupWidget.h
  #../include/mvdDatabaseBrowserController.h
  #../include/mvdDatabaseBrowserWidget.h
  #../include/mvdDatabaseBrowserWidgetTest.h
  #../include/mvdDatabaseTreeWidget.h
  #../include/mvdDatasetCreationProgressDialog.h
  #../include/mvdDatasetDragAndDropEventFilter.h
  #../include/mvdDatasetPropertiesController.h
  #../include/mvdDatasetPropertiesWidget.h
  ../include/mvdDoubleValidator.h
  ../include/mvdDropLineEdit.h
  ../include/mvdFilenameDragAndDropEventFilter.h
  #mvdGui.h
  ../include/mvdHistogramController.h
  ../include/mvdHistogramPlotPicker.h
  ../include/mvdHistogramWidget.h
  ../include/mvdI18nApplication.h
  ../include/mvdI18nMainWindow.h
  ../include/mvdImageViewManipulator.h
  ../include/mvdImageViewRenderer.h
  ../include/mvdImageViewWidget.h
  ../include/mvdImportImagesDialog.h
  ../include/mvdImportSubDatasetDialog.h
  ../include/mvdKeymapDialog.h
  ../include/mvdLayerStackController.h
  ../include/mvdLayerStackItemModel.h
  ../include/mvdLayerStackWidget.h
  ../include/mvdMainWindowTitleLoader.h
  ../include/mvdMultiResolutionPyramidWidget.h
  #mvdMyWidget.h
  ../include/mvdPixelDescriptionWidget.h
  ../include/mvdProjectionBarWidget.h
  ../include/mvdQuicklookViewManipulator.h
  ../include/mvdQuicklookViewRenderer.h
  ../include/mvdSearchableTreeWidget.h
  ../include/mvdStatusBarWidget.h
  ../include/mvdTaskProgressDialog.h
  ../include/mvdTreeWidget.h
  #mvdTreeWidgetItem.h
  ../include/mvdTreeWidgetItemDragAndDropEventFilter.h
  ../include/mvdShaderWidget.h
)

#############################################################################
set( OTBMonteverdiGUI_FORMS
  mvdAboutDialog.ui
  mvdColorBandDynamicsWidget.ui
  mvdColorDynamicsWidget.ui
  mvdColorSetupWidget.ui
  #mvdDatabaseBrowserWidget.ui
  #mvdDatabaseBrowserWidgetTest.ui
  #mvdDatasetCreationProgressDialog.ui
  #mvdDatasetPropertiesWidget.ui
  mvdHistogramWidget.ui
  mvdImportImagesDialog.ui
  mvdImportSubDatasetDialog.ui
  mvdKeymapDialog.ui
  mvdLayerStackWidget.ui
  mvdMultiResolutionPyramidWidget.ui
  #mvdMyWidget.ui
  mvdPixelDescriptionWidget.ui
  mvdProjectionBarWidget.ui
  mvdSearchableTreeWidget.ui
  mvdShaderWidget.ui
  mvdStatusBarWidget.ui
  mvdVideoColorToolBox.ui
)

#############################################################################
set( OTBMonteverdiGUI_RESOURCES
  mvdIcons.qrc
)

#############################################################################
if( OTBQtWidget_ENABLED )
  list( APPEND OTBMonteverdiGUI_SRCS
    mvdApplicationLauncher.cxx
    mvdApplicationsToolBox.cxx
    mvdApplicationsToolBoxController.cxx
    mvdOTBApplicationsModel.cxx
    mvdQtWidgetView.cxx
  )

  list( APPEND OTBMonteverdiGUI_HEADERS_MOC
    ../include/mvdApplicationLauncher.h
    ../include/mvdApplicationsToolBox.h
    ../include/mvdApplicationsToolBoxController.h
    ../include/mvdOTBApplicationsModel.h
    ../include/mvdQtWidgetView.h
    )

  list( APPEND OTBMonteverdiGUI_FORMS
    mvdApplicationsToolBox.ui
    )

  list( APPEND OTBMonteverdiGUI_RESOURCES
    # mvdApplicationsToolBox.qrc
    )
endif()

#############################################################################
qt5_wrap_cpp( OTBMonteverdiGUI_SRC_MOC ${OTBMonteverdiGUI_HEADERS_MOC} OPTIONS -p "." )
qt5_wrap_ui( OTBMonteverdiGUI_FORMS_HEADERS ${OTBMonteverdiGUI_FORMS} )
qt5_add_resources( OTBMonteverdiGUI_RESOURCES_RCC ${OTBMonteverdiGUI_RESOURCES} )

#############################################################################
add_to_qt_i18n_sources( ${OTBMonteverdiGUI_SRCS} )
add_to_qt_i18n_headers( "../include" ${OTBMonteverdiGUI_SRCS} )
add_to_qt_i18n_forms( ${OTBMonteverdiGUI_FORMS} )

#############################################################################
add_library( OTBMonteverdiGUI
  ${OTBMonteverdiGUI_SRCS}
  ${OTBMonteverdiGUI_FORMS_HEADERS}
  ${OTBMonteverdiGUI_SRC_MOC}
  ${OTBMonteverdiGUI_RESOURCES_RCC}
)

#############################################################################
target_link_libraries( OTBMonteverdiGUI
  ${OTBCommon_LIBRARIES}
  ${OTBIce_LIBRARIES}
  ${OTBIOGDAL_LIBRARIES}
  ${OTBMonteverdiCore_LIBRARIES}
  ${OTBQtWidget_LIBRARIES}
  ${OTBQwt_LIBRARIES}
  )

#############################################################################
otb_module_target( OTBMonteverdiGUI )
