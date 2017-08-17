function(install_python_bindings)
  if(HAVE_PYTHON)
    install(DIRECTORY ${SUPERBUILD_INSTALL_DIR}/lib/otb/python
      DESTINATION ${PKG_STAGE_DIR}/lib
      PATTERN "*.pyc" EXCLUDE
      )
  endif()
endfunction()
