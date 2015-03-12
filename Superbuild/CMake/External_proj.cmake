set(proj PROJ)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup Proj.4 ...")

if(USE_SYSTEM_PROJ)
  # TODO : FindPROJ.cmake
  #find_package ( PROJ REQUIRED )
  add_custom_target(${proj})
  message(STATUS "  Using Proj4 system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using Proj4 SuperBuild version")
  
  if(MSVC)

    ExternalProject_Add(${proj}_build
      PREFIX ${proj}
      URL "http://download.osgeo.org/proj/proj-4.8.0.tar.gz"
      URL_MD5 d815838c92a29179298c126effbb1537
      SOURCE_DIR ${PROJ_SB_SRC}
      BINARY_DIR ${PROJ_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      CONFIGURE_COMMAND ""
      BUILD_COMMAND nmake /f ${PROJ_SB_SRC}/makefile.vc
      INSTALL_COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt
      ${CMAKE_BINARY_DIR}/${proj}/_install
    )
    
    ExternalProject_Add(${proj}
      PREFIX ${proj}/_install
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${proj}/_install
      BINARY_DIR ${PROJ_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DPROJ4_BUILD_DIR:STRING=${PROJ_SB_BUILD_DIR}/src
      DEPENDS ${proj}_build
      CMAKE_COMMAND
    )
    
    ExternalProject_Add_Step(${proj}_build copy_source
      COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${PROJ_SB_SRC} ${PROJ_SB_BUILD_DIR}
      DEPENDEES patch update
      DEPENDERS configure
    )
  else()
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://download.osgeo.org/proj/proj-4.8.0.tar.gz"
      URL_MD5 d815838c92a29179298c126effbb1537
      BINARY_DIR ${PROJ_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      CONFIGURE_COMMAND
        # use 'env' because CTest launcher doesn't perform shell interpretation
        env ${LDLIBVAR}=${SB_INSTALL_PREFIX}/lib
        ${PROJ_SB_BUILD_DIR}/configure 
        --prefix=${SB_INSTALL_PREFIX}
        --enable-static=no
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install
    )

    ExternalProject_Add_Step(${proj} copy_source
      COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${PROJ_SB_SRC} ${PROJ_SB_BUILD_DIR}
      DEPENDEES patch update
      DEPENDERS configure
    )    
  endif()
  
  set(${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/proj_i.lib)
  elseif(UNIX)
    set(${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libproj${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()
  
endif()
endif()
