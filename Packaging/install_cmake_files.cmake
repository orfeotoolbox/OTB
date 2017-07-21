include(patch_cmake_files.cmake)

function(install_cmake_files)
 patch_cmake_files(NAME "ITK"
   VERSION "${PKG_ITK_SB_VERSION}"
   MATCH_STRING  "${CMAKE_INSTALL_PREFIX}"
   REPLACE_VAR "ITK_INSTALL_PREFIX"
   )
 
 patch_cmake_files(NAME "OTB"
   VERSION "${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}"
   MATCH_STRING  "${CMAKE_INSTALL_PREFIX}"
   REPLACE_VAR "OTB_INSTALL_PREFIX"
   )
endfunction()
