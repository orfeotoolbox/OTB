#############################################################################
# Source files of Common library
set( Common_SOURCES 
        mvdAboutDialog.cxx
        mvdAbstractImageModel.cxx
        mvdAbstractModelController.cxx
        mvdAbstractModel.cxx
        mvdAbstractViewManipulator.cxx
        mvdAlgorithm.cxx
        mvdColorBandDynamicsWidget.cxx
        mvdColorDynamicsController.cxx
        mvdColorDynamicsWidget.cxx
        mvdColorSetupController.cxx
        mvdColorSetupWidget.cxx
        mvdDatasetCreationProgressDialog.cxx
        mvdDatasetDescriptor.cxx
        mvdDatasetModel.cxx
        mvdGLImageWidget.cxx
        mvdGui.cxx
        mvdHistogramModel.cxx
        mvdI18nApplication.cxx
        mvdImageLoader.cxx
        mvdImageModelRenderer.cxx
        mvdImageViewManipulator.cxx
        mvdMath.cxx
        mvdPreferencesDialog.cxx
        mvdQuicklookModel.cxx
        mvdQuicklookViewManipulator.cxx
        mvdStatusBarWidget.cxx
        mvdSystemError.cxx
        mvdTypes.cxx
        mvdVectorImageModel.cxx
)

# Headers for classes with Q_OBJECT macro (needs to go to Qt moc)
set( Common_HEADERS_MOC
        mvdAboutDialog.h
        mvdAbstractImageModel.h
        mvdAbstractModelController.h
        mvdAbstractModel.h
        mvdAbstractModelRenderer.h
        mvdAbstractViewManipulator.h
        mvdColorBandDynamicsWidget.h
        mvdColorDynamicsController.h
        mvdColorDynamicsWidget.h
        mvdColorSetupController.h
        mvdColorSetupWidget.h
        mvdDatasetCreationProgressDialog.h
        mvdDatasetDescriptor.h
        mvdDatasetModel.h
        mvdGLImageWidget.h
        mvdHistogramModel.h
        mvdI18nApplication.h
        mvdImageLoader.h
        mvdImageModelRenderer.h
        mvdImageViewManipulator.h
        mvdPreferencesDialog.h
        mvdQuicklookModel.h
        mvdQuicklookViewManipulator.h
        mvdStatusBarWidget.h
        mvdVectorImageModel.h
)

# Qt designer UI files
set( Common_FORMS
        mvdAboutDialog.ui
        mvdColorBandDynamicsWidget.ui
        mvdColorDynamicsWidget.ui
        mvdColorSetupWidget.ui
        mvdDatasetCreationProgressDialog.ui
        mvdMyWidget.ui
        mvdPreferencesDialog.ui
        mvdStatusBarWidget.ui
        mvdVideoColorToolBox.ui
)

# Qt resources
set( Common_RESOURCES
)

#############################################################################
# Qt4 wrapped files.
qt4_wrap_cpp( Common_SRC_MOC ${Common_HEADERS_MOC} )
qt4_wrap_ui( Common_FORMS_HEADERS ${Common_FORMS}  )
qt4_add_resources( Common_RESOURCES_RCC ${Common_RESOURCES} )

########################################################################
# Qt4 translations.

# Sources files to translate.
add_to_qt4_i18n_sources( 
  ${Common_SOURCES}
  ${Common_HEADERS_MOC}
  ${Common_SRC_MOC}
  ${Common_FORMS}
)

#############################################################################
add_library(Monteverdi2_Common
  ${Common_SOURCES}
  ${Common_FORMS_HEADERS}
  ${Common_SRC_MOC}
  ${Common_RESOURCES_RCC}
)

#############################################################################
target_link_libraries(Monteverdi2_Common
  OTBIO
  ${OPENGL_LIBRARIES}
  ${QT_LIBRARIES}
)

#############################################################################
# INSTALL Common application.
install(
  TARGETS Monteverdi2_Common
  RUNTIME DESTINATION ${Monteverdi2_INSTALL_BIN_DIR} COMPONENT Runtime
  LIBRARY DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Development
)

#############################################################################
