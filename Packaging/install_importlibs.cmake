# This is more messed up that you might think.
# On windows we cannot track dependency on .lib files like dll
# In linux, checking all required .so file is enough to have
# package with all required libraries. But the issue is not all
# libs files are appearing in objdump's NEEDED section because
# they are not used by the binary in question. This leads us to
# another heap of mess that cmake's Targets-release.cmake references
# them but not included in package. this cmake function is the
# after effect of this issue
#TODO: remove commented code
function(install_importlibs)

  if(WIN32)
    #ALL .lib files are required for building
    #eg: gdal.dll will have gdal_i.lib which is
    #needed when you do cmake configure for projects
    #using OTB. Hence the *.lib regex is non optional
    #and cannot be optimized at any level.
    #Only thing you can control is to create only those
    #required .lib files in install directory. This exclude
    #all otb*.*testdriver.lib etc..
    file(GLOB import_lib_files
      "${SUPERBUILD_INSTALL_DIR}/lib/*.lib")
    foreach( import_lib_file ${import_lib_files})
      install_rule(${import_lib_file})
    endforeach()
    return()
  endif()

  set(search_regex    "..IMPORTED_LOCATION_([A-Z]+)...{_IMPORT_PREFIX}\/lib\/")
  file(GLOB_RECURSE
    target_cmake_files
    "${SUPERBUILD_INSTALL_DIR}/lib/cmake/ITK*/*Targets-*.*"
    "${SUPERBUILD_INSTALL_DIR}/lib/cmake/OTB*/*Targets-*.*")

  set(lib_files)
  foreach( target_cmake_file ${target_cmake_files})
    if(PKG_DEBUG)
      message("checking ${search_regex} in ${target_cmake_file}")
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
