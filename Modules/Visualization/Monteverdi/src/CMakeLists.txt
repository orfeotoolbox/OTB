set( Monteverdi_SRCS
  main.cxx
  mvdApplication.cxx
  mvdMainWindow.cxx
  mvdPreferencesDialog.cxx
  )

add_library( Monteverdi ${Monteverdi_SRCS} )

target_link_libraries( Monteverdi
  ${Monteverdi_LIBRARIES}
  ${MonteverdiCore_LIBRARIES}
  ${MonteverdiGui_LIBRARIES}
  )

add_executable( monteverdi main.cxx )

target_link_libraries( monteverdi Monteverdi )

otb_module_target( Monteverdi )
