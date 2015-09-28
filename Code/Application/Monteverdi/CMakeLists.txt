#############################################################################
# Input files.
set( Monteverdi_App_SOURCES
  mvdApplication.cxx
  mvdMainWindow.cxx
  mvdPreferencesDialog.cxx
  )

# Headers for classes with Q_OBJECT macro (needs to go to Qt moc)
set( Monteverdi_App_HEADERS_MOC
  mvdApplication.h
  mvdMainWindow.h
  mvdPreferencesDialog.h
  )

# Qt designer UI files
set( Monteverdi_App_FORMS
  mvdMainWindow.ui
  mvdPreferencesDialog.ui
  )

# Qt resources included in executable
set( Monteverdi_App_RESOURCES
  mvdMainWindow.qrc
  )

# General include directories.
include_directories(
  ${ICE_INCLUDE_DIRS}
  ${QWT_INCLUDE_DIRS}
  #
  ${Monteverdi_BINARY_DIR}
  #
  ${Monteverdi_SOURCE_DIR}/Code/Common
  #
  ${Monteverdi_SOURCE_DIR}/Code/Application/Monteverdi
  ${Monteverdi_BINARY_DIR}/Code/Application/Monteverdi
  #
  ${Monteverdi_SOURCE_DIR}/Data/Icons
  )

#############################################################################
# Qt4 wrapped files.
qt4_wrap_cpp( Monteverdi_App_SRC_MOC ${Monteverdi_App_HEADERS_MOC} )
qt4_wrap_ui( Monteverdi_App_FORMS_HEADERS ${Monteverdi_App_FORMS}  )
qt4_add_resources( Monteverdi_App_RESOURCES_RCC ${Monteverdi_App_RESOURCES} )

#############################################################################
# Sources files to translate.
add_to_qt4_i18n_sources(
  main.cxx
  ${Monteverdi_App_SOURCES}
  ${Monteverdi_App_HEADERS_MOC}
  ${Monteverdi_App_FORMS}
  )

if (WIN32)
  # Windows Resource file need to have the full path to icon
  # if the .ico is not in the current dir
  set( Monteverdi_App_WIN32_ICON ${Monteverdi_SOURCE_DIR}/Data/Icons/monteverdi.ico)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/mvdWin32.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/mvdWin32.rc
    @ONLY
    )

  if (MINGW)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
  endif()

  # The RC file to be added to source list
  set( Monteverdi_App_WIN32_RC_FILE ${CMAKE_CURRENT_BINARY_DIR}/mvdWin32.rc )
endif()


#############################################################################
# Monteverdi_App library
# STATIC so that the monteverdi executable does not depend on another shared lib
# The library is also used in tests, waiting for a better test strategy
# The library is not installed

add_library( Monteverdi_App
  STATIC
  ${Monteverdi_App_SOURCES}
  ${Monteverdi_App_FORMS_HEADERS}
  ${Monteverdi_App_SRC_MOC}
  )
target_link_libraries( Monteverdi_App
  Monteverdi_Gui
  )

if (OTBQtWidget_LOADED)
  target_link_libraries( Monteverdi_App
    Monteverdi_ApplicationsWrapper
    )
endif()

create_monteverdi_application(NAME monteverdi
  SOURCES main.cxx
  ${Monteverdi_App_WIN32_RC_FILE}
  ${Monteverdi_App_RESOURCES_RCC}
  LINK_LIBRARIES Monteverdi_App
  ${QT_QTMAIN_LIBRARY}
  OUTPUT_NAME ${MV2_OUTPUT_NAME}
  COMPONENT_NAME Monteverdi
  NEEDS_OTB_APPS FALSE
  )
