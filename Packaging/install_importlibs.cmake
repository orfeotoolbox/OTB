# This is more messed up that you might think.
# On windows we cannot track dependency on .lib files like dll
function(install_importlibs)

  if(WIN32)
    #ALL .lib files are required for building
    #eg: gdal.dll will have gdal_i.lib which is
    #needed when you do cmake configure for projects
    #using OTB. Hence the *.lib regex is non optional
    #and cannot be optimized at any level.
    #Only thing you can control is to create only those
    #required .lib files in install directory.
    file(GLOB import_lib_files
      "${SUPERBUILD_INSTALL_DIR}/lib/*.lib")
    foreach( import_lib_file ${import_lib_files})
      install_rule(${import_lib_file})
    endforeach()
    return()
  endif()
endfunction()
