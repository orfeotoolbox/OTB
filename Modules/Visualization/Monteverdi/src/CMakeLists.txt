set( OTBMonteverdi_SRCS
  main.cxx
  mvdApplication.cxx
  mvdMainWindow.cxx
  mvdPreferencesDialog.cxx
  )

add_executable( monteverdi ${OTBMonteverdi_SRCS} )

target_link_libraries( monteverdi
  ${OTBMonteverdiCore_LIBRARIES}
  ${OTBMonteverdiGUI_LIBRARIES}
  )

otb_module_target( Monteverdi )
