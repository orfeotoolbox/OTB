#############################################################################
set( OTBMapla_SRCS
  main.cxx
  MaplaApplication.cxx
  MaplaMainWindow.cxx
  )

#############################################################################
set( OTBMapla_HEADERS_MOC
       mvdMaplaApplication.h
       mvdMaplaMainWindow.h
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

#############################################################################
qt4_wrap_cpp( OTBMapla_SRC_MOC ${OTBMapla_HEADERS_MOC} )
qt4_wrap_ui( OTBMapla_FORMS_HEADERS ${OTBMapla_FORMS}  )
qt4_add_resources( OTBMapla_RESOURCES_RCC ${OTBMapla_RESOURCES} )

#############################################################################
add_executable( mapla ${OTBMapla_SRCS} )

#############################################################################
target_link_libraries( mapla
  ${OTBMonteverdiCore_LIBRARIES}
  ${OTBMonteverdiGUI_LIBRARIES}
  )

#############################################################################
otb_module_target( mapla )
