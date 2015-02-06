message(STATUS "Setup Proj.4 ...")

set(proj PROJ)

set(DEFAULT_USE_SYSTEM_PROJ  OFF)

option(USE_SYSTEM_PROJ "  Use a system build of Proj.4." ${DEFAULT_USE_SYSTEM_PROJ})
mark_as_advanced(USE_SYSTEM_PROJ)

if(USE_SYSTEM_PROJ)
  message(STATUS "  Using Proj4 system version")
else()
  
  set(PROJ_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(PROJ_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  if(MSVC)

    ExternalProject_Add(${proj}_build
      PREFIX ${proj}
      URL "http://download.osgeo.org/proj/proj-4.8.0.tar.gz"
      URL_MD5 d815838c92a29179298c126effbb1537
      SOURCE_DIR ${PROJ_SB_SRC}
      BINARY_DIR ${PROJ_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
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
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
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
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND
        # use 'env' because CTest launcher doesn't perform shell interpretation
        env ${LDLIBVAR}=${CMAKE_INSTALL_PREFIX}/lib
        ${PROJ_SB_BUILD_DIR}/configure 
        --prefix=${CMAKE_INSTALL_PREFIX}
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
  
  message(STATUS "  Using Proj4 SuperBuild version")
endif()
