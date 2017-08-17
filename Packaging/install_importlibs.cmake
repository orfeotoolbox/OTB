#TODO: remove commented code
function(install_importlibs)

  if(NOT WIN32)
    return()
  endif()

  set(search_regex  "..IMPORTED_IMPLIB_([A-Z]+)...{_IMPORT_PREFIX}\/lib\/")
  if(NOT WIN32)
    set(search_regex    "..IMPORTED_LOCATION_([A-Z]+)...{_IMPORT_PREFIX}\/lib\/")
  endif()

  file(GLOB_RECURSE
    target_cmake_files
    "${SUPERBUILD_INSTALL_DIR}/lib/cmake/ITK*/*Targets-*.*"
    "${SUPERBUILD_INSTALL_DIR}/lib/cmake/OTB*/*Targets-*.*")

  set(lib_files)
  foreach( target_cmake_file ${target_cmake_files})
    if(PKG_DEBUG)
      message("checking IMPORTED_IMPLIB_RELEASE in ${target_cmake_file}")
    endif()
    file(STRINGS "${target_cmake_file}" matched_items REGEX "${search_regex}")
    foreach(matched_item ${matched_items})
      if(matched_item MATCHES "otbapp_")
	continue()
      endif()
      string(REGEX REPLACE "${search_regex}" "" lib_file "${matched_item}")
      string(REGEX REPLACE "\"" "" lib_file "${lib_file}")
      list(APPEND lib_files "${SUPERBUILD_INSTALL_DIR}/lib/${lib_file}")
    endforeach()
  endforeach()

  foreach(lib_file ${lib_files})
    if(PKG_DEBUG)
      message("SPECIAL install_rule for lib_file: ${lib_file}")
    endif()
    install_rule(${lib_file})
  endforeach()


#RK: to hell with cmake targets files.
  # file(GLOB ALL_EXTRA_FILES
    # ${SUPERBUILD_INSTALL_DIR}/lib/*boost*${LIB_EXT}*
    # ${SUPERBUILD_INSTALL_DIR}/lib/*glut*${LIB_EXT}*
    # ${SUPERBUILD_INSTALL_DIR}/lib/*QtXml*${LIB_EXT}*
    # ${SUPERBUILD_INSTALL_DIR}/lib/*kml*${LIB_EXT}*
      # )

      
    # file(GLOB MY_LIB_FILES 
    # "${SUPERBUILD_INSTALL_DIR}/lib/*${STATIC_LIB_EXT}"
    # "${SUPERBUILD_INSTALL_DIR}/${DEST_LIB_DIR}/${LIB_PREFIX}itk*${LIB_EXT}*"
    # "${SUPERBUILD_INSTALL_DIR}/${DEST_LIB_DIR}/${LIB_PREFIX}ITK*${LIB_EXT}*"
    # )
    # foreach(MY_LIB_FILE ${MY_LIB_FILES})
    # set(a_symlink FALSE)
    #  isfile_symlink("${itk_lib_file}" a_symlink itk_lib_file_target)
    #  if(a_symlink)
    #       #TODO
    #  else()
    #    install_rule(${MY_LIB_FILE})
    #    #list(APPEND file_list "${itk_lib_file}")
    #  endif()
     
      #get_filename_component(MY_LIB_FILE_name ${MY_LIB_FILE} NAME)
      #list(APPEND file_list "${MY_LIB_FILE_name}")
    
    #endforeach()  

    #RK: there is a bug in itk cmake files in install tree 
    #we workaround with below code
    #start hack
     # file(GLOB itk_all_lib_files  )

   # foreach(itk_lib_file ${itk_all_lib_files})
   # set(a_symlink FALSE)
     # isfile_symlink("${itk_lib_file}" a_symlink itk_lib_file_target)
     # if(NOT a_symlink)
       # list(APPEND file_list "${itk_lib_file}")
     # endif()
   # endforeach()
   #end hack
endfunction()
