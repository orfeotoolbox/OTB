set( OTBMonteverdi_SRCS
  mvdApplication.cxx
  mvdMainWindow.cxx
  mvdPreferencesDialog.cxx
  )

#############################################################################
set( OTBMonteverdi_HEADERS_MOC
  ../include/mvdApplication.h
  ../include/mvdMainWindow.h
  ../include/mvdPreferencesDialog.h
  )

#############################################################################
# Qt designer UI files
set( OTBMonteverdi_FORMS
  mvdMainWindow.ui
  mvdPreferencesDialog.ui
  )

#############################################################################
set( OTBMonteverdi_RESOURCES
  mvdMainWindow.qrc
  )

#############################################################################
qt4_wrap_cpp( OTBMonteverdi_SRC_MOC ${OTBMonteverdi_HEADERS_MOC} )
qt4_wrap_ui( OTBMonteverdi_FORMS_HEADERS ${OTBMonteverdi_FORMS}  )
qt4_add_resources( OTBMonteverdi_RESOURCES_RCC ${OTBMonteverdi_RESOURCES} )


add_library( OTBMonteverdi
  STATIC
  ${OTBMonteverdi_SRCS}
  ${OTBMonteverdi_FORMS_HEADERS}
  ${OTBMonteverdi_SRC_MOC}
  )
target_link_libraries( OTBMonteverdi
  ${OTBMonteverdiCore_LIBRARIES}
  ${OTBMonteverdiGUI_LIBRARIES}
  ${OTBQt4_LIBRARIES}
  )
otb_module_target( OTBMonteverdi )

#######################[ Monteverdi executable ]#############################
if (WIN32)
  # Windows Resource file need to have the full path to icon
  # if the .ico is not in the current dir
  set( Monteverdi_App_WIN32_ICON ${Monteverdi_SOURCE_DIR}/Data/Icons/monteverdi.ico)
  configure_file(
    mvdWin32.rc.in
    mvdWin32.rc
    @ONLY
    )

  if (MINGW)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
  endif()

  # The RC file to be added to source list
  set( Monteverdi_App_WIN32_RC_FILE ${CMAKE_CURRENT_BINARY_DIR}/mvdWin32.rc )
endif()

add_executable( monteverdi
  main.cxx
  ${Monteverdi_App_WIN32_RC_FILE}
  ${OTBMonteverdi_RESOURCES_RCC}
  )
target_link_libraries( monteverdi
  OTBMonteverdi
  )
otb_module_target( monteverdi )
