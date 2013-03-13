#############################################################################
# Input files.
set( Application_SOURCES
       mvdApplication.cxx
       mvdImageLoader.cxx
       mvdImageModel.cxx
       mvdMainWindow.cxx
       mvdPreferencesDialog.cxx
)

# Headers for classes with Q_OBJECT macro (needs to go to Qt moc)
set( Application_HEADERS_MOC
       mvdApplication.h
       mvdImageModel.h
       mvdImageLoader.h
       mvdMainWindow.h
       mvdPreferencesDialog.h
)

# Qt designer UI files
set( Application_FORMS
       mvdMainWindow.ui
       mvdPreferencesDialog.ui
)

# Qt resources included in executable
set( Application_RESOURCES
       mvdMainWindow.qrc
)

# General include directories.
include_directories(
  ${Monteverdi2_BINARY_DIR}

  ${Monteverdi2_SOURCE_DIR}/Code/Common

  ${Monteverdi2_SOURCE_DIR}/Code/Application/Viewer
  ${Monteverdi2_BINARY_DIR}/Code/Application/Viewer

  ${Monteverdi2_SOURCE_DIR}/Data/Icons
)

#############################################################################
# Qt4 wrapped files.
qt4_wrap_cpp( Application_SRC_MOC ${Application_HEADERS_MOC} )
qt4_wrap_ui( Application_FORMS_HEADERS ${Application_FORMS}  )
qt4_add_resources( Application_RESOURCES_RCC ${Application_RESOURCES} )

#############################################################################
# Sources files to translate.
add_to_qt4_i18n_sources(
  ${Application_SOURCES}
  ${Application_HEADERS_MOC}
  ${Application_FORMS}
)

if (WIN32)
    # Windows Resource file need to have the full path to icon
    # if the .ico is not in the current dir
    set( Monteverdi2_WIN32_ICON ${Monteverdi2_SOURCE_DIR}/Data/Icons/monteverdi2.ico)
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/mvdWin32.rc.in
      ${CMAKE_CURRENT_BINARY_DIR}/mvdWin32.rc
      @ONLY
    )
    
    # The RC file to be added to source list
    set( Monteverdi2_WIN32_RC_FILE ${CMAKE_CURRENT_BINARY_DIR}/mvdWin32.rc )
endif()


#############################################################################
# Monteverdi2_Application library
# STATIC so that the monteverdi2 executable does not depend on another shared lib
# The library is also used in tests, waiting for a better test strategy
# The library is not installed

add_library( Monteverdi2_Application
  STATIC
  ${Application_SOURCES}
  ${Application_FORMS_HEADERS}
  ${Application_SRC_MOC}
)
target_link_libraries( Monteverdi2_Application
  Monteverdi2_Common
)

#############################################################################
# monteverdi2 executable
add_executable( mvd2-viewer
  WIN32
  main.cxx
  ${Monteverdi2_WIN32_RC_FILE}
  ${Application_RESOURCES_RCC}
)

target_link_libraries( mvd2-viewer
  Monteverdi2_Application
  Monteverdi2_Common
  ${QT_QTMAIN_LIBRARY}
)

# deploy a qt.conf file in the monteverdi2 target build dir
# to avoid warning on translation discovery
deploy_qt_conf( mvd2-viewer )

#############################################################################
install(
  TARGETS mvd2-viewer
  RUNTIME DESTINATION ${Monteverdi2_INSTALL_BIN_DIR} COMPONENT Runtime
  LIBRARY DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Development
)

#############################################################################
