set(proj PNG)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup libpng...")

if(USE_SYSTEM_PNG)
  find_package ( PNG )
  add_custom_target(${proj})
  message(STATUS "  Using libpng system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using libpng SuperBuild version")
  
  # declare dependencies
  set(${proj}_DEPENDENCIES ZLIB)
  INCLUDE_SUPERBUILD_DEPENDENCIES(${${proj}_DEPENDENCIES})
  # set proj back to its original value
  set(proj PNG)
  
  ADD_SUPERBUILD_CMAKE_VAR(ZLIB_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(ZLIB_LIBRARY)
  
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://sourceforge.net/projects/libpng/files/libpng16/1.6.16/lpng1616.zip/download"
      URL_MD5 c90c9587c9a5c735327fb3f6900f6b03
      BINARY_DIR ${PNG_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DEPENDS ${${proj}_DEPENDENCIES}
      CMAKE_CACHE_ARGS
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DCMAKE_INSTALL_PREFIX:PATH=${SB_INSTALL_PREFIX}
        -DSKIP_INSTALL_EXECUTABLES:BOOL=OFF
        -DPNG_STATIC:BOOL=OFF
        -DPNG_SHARED:BOOL=ON
        -DPNG_TESTS:BOOL=OFF
        -DCMAKE_PREFIX_PATH:STRING=${SB_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}
        ${PNG_SB_CONFIG}        
      CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )
    #having -DPNGLIB_NAME:STRING=libpng in CMAKE_CACHE_ARGS doesnt work
    #libpng library built in the name libpng16.lib and other libs (Qt4) 
    #reports libpng.lib is not found As Qt is strict on the name of libpng
    #below we copy the libpng16.lib to libpng.lib.
    #modifying CMakeLists.txt in libpng is another way but then whole 
    #CMakeLists.txt must be kept in superbuild repository.
    if(MSVC)
      ExternalProject_Add_Step(${proj} duplicate_pnglib
        COMMAND ${CMAKE_COMMAND} -E copy
        ${SB_INSTALL_PREFIX}/lib/libpng16.lib ${SB_INSTALL_PREFIX}/lib/libpng.lib
        DEPENDEES install
    )    
    endif()
  
  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libpng.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libpng${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()
  
endif()
endif()
