function(install_mvd_files)
  if(NOT HAVE_MVD)
    return()
  endif()

  #install icon file for .app file. Monteverdi and Mapla has same icon!
  if(APPLE)
    install(FILES Files/Monteverdi.icns
      DESTINATION ${PKG_STAGE_DIR})
  endif()

  #TODO:? revisit this
  if(UNIX)
    file(GLOB DOT_A_FILES ${SUPERBUILD_INSTALL_DIR}/lib/lib*.a )
    foreach(DOT_A_FILE ${DOT_A_FILES})
      install(FILES "${DOT_A_FILE}" DESTINATION ${PKG_STAGE_DIR}/lib)
    endforeach()
  endif()
endfunction()
