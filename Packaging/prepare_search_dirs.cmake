function(prepare_search_dirs search_dirs_result)
  
  set(search_dirs)
  
  if(MSVC)
    if(DEFINED ENV{UniversalCRTSdkDir})
      message(FATAL_ERROR "UniversalCRTSdkDir variable not set. Cannot continue")
    endif()
    if(NOT DEFINED ENV{VCINSTALLDIR})
      message(FATAL_ERROR  "VCINSTALLDIR variable not set. Cannot continue.")
    endif()
  endif()
  
  if(MSVC)    
    file(TO_CMAKE_PATH "$ENV{UniversalCRTSdkDir}" UCRT_SDK_DIR)
    list(APPEND search_dirs "${UCRT_SDK_DIR}/Redist/ucrt/DLLs/${OTB_TARGET_SYSTEM_ARCH}") #ucrt dlls
  
  #additional msvc redist dll from VCINSTALLDIR
  file(TO_CMAKE_PATH "$ENV{VCINSTALLDIR}" PKG_VCINSTALLDIR)
  list(APPEND search_dirs 
    "${PKG_VCINSTALLDIR}/redist/${OTB_TARGET_SYSTEM_ARCH}/Microsoft.VC140.CRT"
    "${PKG_VCINSTALLDIR}/redist/${OTB_TARGET_SYSTEM_ARCH}/Microsoft.VC140.OPENMP"
    )
endif()

#superbuild .so /.dylib
list(APPEND search_dirs "${SUPERBUILD_INSTALL_DIR}/lib")
#all executables gdalinfo etc..
list(APPEND search_dirs "${SUPERBUILD_INSTALL_DIR}/bin") 
#common for all platforms.
list(APPEND search_dirs "${SUPERBUILD_INSTALL_DIR}/lib/otb/applications")

#_otbApplication.so
list(APPEND search_dirs "${SUPERBUILD_INSTALL_DIR}/lib/otb/python")

#for otbtest executables. 
list(APPEND search_dirs ${OTB_BINARY_DIR}/bin)

set(${search_dirs_result} ${search_dirs} PARENT_SCOPE)
endfunction()
