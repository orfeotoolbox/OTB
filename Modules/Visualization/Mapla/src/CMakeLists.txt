set( OTBMapla_SRCS
  main.cxx
  maplaApplication.cxx
  maplaMainWindow.cxx
  )

add_executable( mapla ${OTBMapla_SRCS} )

target_link_libraries( mapla
  ${OTBMonteverdiCore_LIBRARIES}
  ${OTBMonteverdiGUI_LIBRARIES}
  )

otb_module_target( mapla )
