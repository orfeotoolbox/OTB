function(install_vstudio_files)
if(MSVC)
    install(FILES
      "Files/OTB Project.zip"
      "Files/start_devenv.bat"
      DESTINATION "${PKG_STAGE_DIR}" )
  endif()
endfunction()
