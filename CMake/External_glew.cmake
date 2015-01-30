message(STATUS "Setup glew...")

set(proj GLEW)

set(DEFAULT_USE_SYSTEM_GLEW  ON)

option(USE_SYSTEM_GLEW "  Use system installed GLEW." ${DEFAULT_USE_SYSTEM_GLEW})
mark_as_advanced(USE_SYSTEM_GLEW)

if(USE_SYSTEM_GLEW)
  message(STATUS "  Using glew system version")
else()
  set(GLEW_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(GLEW_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  if(MSVC)
    set(SB_GLEW_DIR "Release/Win32")
    # TODO: add proper check for x64
    # if ${CMAKE_SYSTEM_PROCESSOR} MATCHES "")
    # set(SB_GLEW_DIR "Release/x64")
    # endif()
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "https://sourceforge.net/projects/glew/files/glew/1.12.0/glew-1.12.0-win32.zip/download"
      URL_MD5 13bde2ce4168c1288f8d75ef6e051b56
      BINARY_DIR ${GLEW_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLEW_SB_SRC}/include ${CMAKE_INSTALL_PREFIX}/include      
      CONFIGURE_COMMAND ${CMAKE_COMMAND} -E copy ${GLEW_SB_SRC}/lib/${SB_GLEW_DIR}/glew32s.lib ${CMAKE_INSTALL_PREFIX}/lib
      BUILD_COMMAND  ${CMAKE_COMMAND} -E copy ${GLEW_SB_SRC}/lib/${SB_GLEW_DIR}/glew32.lib ${CMAKE_INSTALL_PREFIX}/lib
      INSTALL_COMMAND  ${CMAKE_COMMAND} -E copy ${GLEW_SB_SRC}/bin/${SB_GLEW_DIR}/glew32.dll ${CMAKE_INSTALL_PREFIX}/bin/
    )
    
    message(STATUS "  Using glew SuperBuild version (prebuilt binaries) ")
  
  else(UNIX)
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "https://sourceforge.net/projects/glew/files/glew/1.12.0/glew-1.12.0.tgz/download"
      URL_MD5 01246c7ecd135d99be031aa63f86dca1
      BINARY_DIR ${GLEW_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CMAKE_CACHE_ARGS
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX} 
    )
    
    message(STATUS "  Using glew SuperBuild version")
 
   endif()
  
endif()
