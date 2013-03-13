#############################################################################
# Input files.
set( Catalogue_SOURCES
       mvdApplication.cxx
       mvdMainWindow.cxx
)

# Headers for classes with Q_OBJECT macro (needs to go to Qt moc)
set( Catalogue_HEADERS_MOC
       mvdApplication.h
       mvdMainWindow.h
)

# Qt designer UI files
set( Catalogue_FORMS
       mvdMainWindow.ui
)

# Qt resources included in executable
set( Catalogue_RESOURCES
       mvdMainWindow.qrc
)

# General include directories.
include_directories(
  ${Monteverdi2_BINARY_DIR}

  ${Monteverdi2_SOURCE_DIR}/Code/Common

  ${Monteverdi2_SOURCE_DIR}/Code/Application/Catalogue
  ${Monteverdi2_BINARY_DIR}/Code/Application/Catalogue

  ${Monteverdi2_SOURCE_DIR}/Data/Icons
)

#############################################################################
# Qt4 wrapped files.
qt4_wrap_cpp( Catalogue_SRC_MOC ${Catalogue_HEADERS_MOC} )
qt4_wrap_ui( Catalogue_FORMS_HEADERS ${Catalogue_FORMS}  )
qt4_add_resources( Catalogue_RESOURCES_RCC ${Catalogue_RESOURCES} )

#############################################################################
# Sources files to translate.
add_to_qt4_i18n_sources(
  ${Catalogue_SOURCES}
  ${Catalogue_HEADERS_MOC}
  ${Catalogue_FORMS}
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
# Monteverdi2_Catalogue library
# STATIC so that the monteverdi2 executable does not depend on another shared lib
# The library is also used in tests, waiting for a better test strategy
# The library is not installed

add_library(Monteverdi2_Catalogue
  STATIC
  ${Catalogue_SOURCES}
  ${Catalogue_FORMS_HEADERS}
  ${Catalogue_SRC_MOC}
)
target_link_libraries(Monteverdi2_Catalogue
  Monteverdi2_Gui
)

#############################################################################
# monteverdi2 executable
add_executable( mvd2-catalogue
  WIN32
  main.cxx
  ${Monteverdi2_WIN32_RC_FILE}
  ${Catalogue_RESOURCES_RCC}
)

target_link_libraries( mvd2-catalogue
  Monteverdi2_Catalogue
  #Monteverdi2_Gui
  ${QT_QTMAIN_LIBRARY}
)

# deploy a qt.conf file in the monteverdi2 target build dir
# to avoid warning on translation discovery
deploy_qt_conf( mvd2-catalogue )

#############################################################################
install(
  TARGETS mvd2-catalogue
  RUNTIME DESTINATION ${Monteverdi2_INSTALL_BIN_DIR} COMPONENT Runtime
  LIBRARY DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Development
)

#############################################################################
