function(install_include_dirs)
  # #install ${SUPERBUILD_INSTALL_DIR}/include directory.
  file(GLOB ALL_IN_INCLUDE_DIR "${SUPERBUILD_INSTALL_DIR}/include/*")
  foreach(INCLUDE_DIR_ITEM ${ALL_IN_INCLUDE_DIR})
    get_filename_component(INCLUDE_DIR_ITEM_name ${INCLUDE_DIR_ITEM} NAME)
    if( IS_DIRECTORY ${INCLUDE_DIR_ITEM})
      install(CODE
        "message(STATUS \"Installing: ${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}/include/${INCLUDE_DIR_ITEM_name}/\")" )
      install(DIRECTORY   "${INCLUDE_DIR_ITEM}"
        DESTINATION "${PKG_STAGE_DIR}/include/"
        MESSAGE_NEVER)
    else()
      install(FILES   "${INCLUDE_DIR_ITEM}"
        DESTINATION "${PKG_STAGE_DIR}/include/" )
    endif() #if( IS_DIRECTORY    
  endforeach()
endfunction()
