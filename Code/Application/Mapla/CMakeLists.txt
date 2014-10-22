#############################################################################
# Input files.
set( Mapla_SOURCES
       mvdApplication.cxx
       mvdMainWindow.cxx
       # mvdPreferencesDialog.cxx
)

# Headers for classes with Q_OBJECT macro (needs to go to Qt moc)
set( Mapla_HEADERS_MOC
       mvdApplication.h
       mvdMainWindow.h
       # mvdPreferencesDialog.h
)

# Qt designer UI files
set( Mapla_FORMS
       mvdMainWindow.ui
       # mvdPreferencesDialog.ui
)

# Qt resources included in executable
set( Mapla_RESOURCES
       mvdMainWindow.qrc
)

# General include directories.
include_directories(
  # ${ICE_INCLUDE_DIRS}
  # ${QWT_INCLUDE_DIRS}
  #
  ${Monteverdi2_BINARY_DIR}
  #
  ${Monteverdi2_SOURCE_DIR}/Code/Common
  #
  ${Monteverdi2_SOURCE_DIR}/Code/Application/Mapla
  ${Monteverdi2_BINARY_DIR}/Code/Application/Mapla
  #
  ${Monteverdi2_SOURCE_DIR}/Data/Icons
)

#############################################################################
# Qt4 wrapped files.
qt4_wrap_cpp( Mapla_SRC_MOC ${Mapla_HEADERS_MOC} )
qt4_wrap_ui( Mapla_FORMS_HEADERS ${Mapla_FORMS}  )
qt4_add_resources( Mapla_RESOURCES_RCC ${Mapla_RESOURCES} )

#############################################################################
# Sources files to translate.
add_to_qt4_i18n_sources(
  main.cxx
  ${Mapla_SOURCES}
  ${Mapla_HEADERS_MOC}
  ${Mapla_FORMS}
)

if (WIN32)
    # Windows Resource file need to have the full path to icon
    # if the .ico is not in the current dir
    set( Mapla_WIN32_ICON ${Monteverdi2_SOURCE_DIR}/Data/Icons/monteverdi2.ico)
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/mvdWin32.rc.in
      ${CMAKE_CURRENT_BINARY_DIR}/mvdWin32.rc
      @ONLY
    )

  if (MINGW)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
  endif()
 
    # The RC file to be added to source list
    set( Mapla_WIN32_RC_FILE ${CMAKE_CURRENT_BINARY_DIR}/mvdWin32.rc )
endif()


#############################################################################
# Mapla library
# STATIC so that the monteverdi2 executable does not depend on another shared lib
# The library is also used in tests, waiting for a better test strategy
# The library is not installed

# add_library( Mapla
#   STATIC
#   ${Mapla_SOURCES}
#   ${Mapla_FORMS_HEADERS}
#   ${Mapla_SRC_MOC}
# )
# target_link_libraries( Mapla
#   Monteverdi2_Gui
# )

# if (OTB_WRAP_QT)
# target_link_libraries( Mapla
#   Monteverdi2_ApplicationsWrapper
# )
# endif()

#############################################################################
# monteverdi2 executable
add_executable( mapla
  WIN32
  main.cxx
  ${Mapla_WIN32_RC_FILE}
  ${Mapla_RESOURCES_RCC}
  #
  ${Mapla_SOURCES}
  ${Mapla_FORMS_HEADERS}
  ${Mapla_SRC_MOC}  
)

if (MONTEVERDI2_OUTPUT_NAME)
  set_target_properties(monteverdi2 PROPERTIES OUTPUT_NAME ${MONTEVERDI2_OUTPUT_NAME})
endif()

target_link_libraries( mapla
  # Mapla
  Monteverdi2_Gui
  ${QT_QTMAIN_LIBRARY}
)

if( OTB_WRAP_QT )
  target_link_libraries( mapla
    Monteverdi2_ApplicationsWrapper
  )
endif()

# deploy a qt.conf file in the monteverdi2 target build dir
# to avoid warning on translation discovery
deploy_qt_conf( monteverdi2 )

#############################################################################
install(
  TARGETS mapla
  RUNTIME DESTINATION ${Monteverdi2_INSTALL_BIN_DIR} COMPONENT Runtime
  LIBRARY DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Development
)

#############################################################################
