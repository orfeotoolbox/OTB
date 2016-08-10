set( OTBMonteverdi_SRCS
  main.cxx
  mvdApplication.cxx
  mvdMainWindow.cxx
  mvdPreferencesDialog.cxx
  )

#############################################################################
set( OTBMonteverdi_HEADERS_MOC
  mvdApplication.h
  mvdMainWindow.h
  mvdPreferencesDialog.h
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

#############################################################################
add_executable( monteverdi ${OTBMonteverdi_SRCS} )

#############################################################################
target_link_libraries( monteverdi
  ${OTBMonteverdiCore_LIBRARIES}
  ${OTBMonteverdiGUI_LIBRARIES}
  )

#############################################################################
otb_module_target( monteverdi )
