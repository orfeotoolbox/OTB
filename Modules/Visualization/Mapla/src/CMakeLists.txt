#############################################################################
set( OTBMapla_SRCS
  mvdMaplaApplication.cxx
  mvdMaplaMainWindow.cxx
  )

#############################################################################
set( OTBMapla_HEADERS_MOC
       ../include/mvdMaplaApplication.h
       ../include/mvdMaplaMainWindow.h
       # mvdPreferencesDialog.h
)

#############################################################################
set( OTBMapla_FORMS
       mvdMaplaMainWindow.ui
       # mvdPreferencesDialog.ui
)

#############################################################################
set( OTBMapla_RESOURCES
       mvdMaplaMainWindow.qrc
)

if (WIN32)
  # Windows Resource file need to have the full path to icon
  # if the .ico is not in the current dir
  set( Mapla_App_WIN32_ICON ${OTB_SOURCE_DIR}/Utilities/Data/Icons/monteverdi.ico)
  configure_file(
    mvdMaplaWin32.rc.in
    mvdMaplaWin32.rc
    @ONLY
    )

  if (MINGW)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
  endif()

  # The RC file to be added to source list
  set( Mapla_App_WIN32_RC_FILE ${CMAKE_CURRENT_BINARY_DIR}/mvdMaplaWin32.rc )
endif()

#############################################################################
qt4_wrap_cpp( OTBMapla_SRC_MOC ${OTBMapla_HEADERS_MOC} )
qt4_wrap_ui( OTBMapla_FORMS_HEADERS ${OTBMapla_FORMS}  )
qt4_add_resources( OTBMapla_RESOURCES_RCC ${OTBMapla_RESOURCES} )

#############################################################################
add_to_qt4_i18n_sources( ${OTBMapla_SRCS} )
add_to_qt4_i18n_headers( "../include" ${OTBMapla_SRCS} )
add_to_qt4_i18n_forms( ${OTBMapla_FORMS} )

##########################[Mapla library]####################################
add_library( OTBMapla
  STATIC
  ${OTBMapla_SRCS}
  ${OTBMapla_FORMS_HEADERS}
  ${OTBMapla_SRC_MOC}
  )
target_link_libraries( OTBMapla
  ${OTBMonteverdiCore_LIBRARIES}
  ${OTBMonteverdiGUI_LIBRARIES}
  ${OTBQt4_LIBRARIES}
  )
otb_module_target( OTBMapla )

############################[Mapla executable]###############################
add_executable( mapla
  main.cxx
  ${Mapla_App_WIN32_RC_FILE}
  ${OTBMapla_RESOURCES_RCC}
  )
target_link_libraries( mapla
  OTBMapla
  )
otb_module_target( mapla )
