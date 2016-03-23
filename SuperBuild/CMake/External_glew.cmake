set(proj GLEW)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup GLEW...")

if(USE_SYSTEM_GLEW)
  find_package ( GLEW )
  message(STATUS "  Using GLEW system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using GLEW SuperBuild version")
  
  if(UNIX)  
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "https://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0.tgz/download"
      URL_MD5 7cbada3166d2aadfc4169c4283701066
      BINARY_DIR ${GLEW_SB_BUILD_DIR}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/patches/${proj}/Makefile ${GLEW_SB_SRC} 
       COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLEW_SB_SRC} ${GLEW_SB_BUILD_DIR}
      CONFIGURE_COMMAND ""
      BUILD_COMMAND $(MAKE) GLEW_PREFIX=${SB_INSTALL_PREFIX} GLEW_DEST=${SB_INSTALL_PREFIX}
      INSTALL_COMMAND $(MAKE) install GLEW_PREFIX=${SB_INSTALL_PREFIX} GLEW_DEST=${SB_INSTALL_PREFIX}
      )
  elseif(MSVC)
    set(SB_GLEW_DIR "Release/Win32")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(SB_GLEW_DIR "Release/x64")
    endif()
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "https://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0-win32.zip/download"
      URL_MD5 a2c0e4d75bdb84217550e51c0f1e7090
      BINARY_DIR ${GLEW_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLEW_SB_SRC}/include ${CMAKE_INSTALL_PREFIX}/include      
      CONFIGURE_COMMAND ${CMAKE_COMMAND} -E copy ${GLEW_SB_SRC}/lib/${SB_GLEW_DIR}/glew32s.lib ${CMAKE_INSTALL_PREFIX}/lib
      BUILD_COMMAND  ${CMAKE_COMMAND} -E copy ${GLEW_SB_SRC}/lib/${SB_GLEW_DIR}/glew32.lib ${CMAKE_INSTALL_PREFIX}/lib
      INSTALL_COMMAND  ${CMAKE_COMMAND} -E copy ${GLEW_SB_SRC}/bin/${SB_GLEW_DIR}/glew32.dll ${CMAKE_INSTALL_PREFIX}/bin/
      )
    
    message(STATUS "  Using glew SuperBuild version (prebuilt binaries) ")
  endif()
  
 set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libGLEW.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libGLEW${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()
  
endif()
endif()
