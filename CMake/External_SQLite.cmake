message(STATUS "Setup SQLite...")

set(proj SQLITE)

set(DEFAULT_USE_SYSTEM_SQLITE  OFF)

option(USE_SYSTEM_SQLITE "  Use a system build of SQLite." ${DEFAULT_USE_SYSTEM_SQLITE})
mark_as_advanced(USE_SYSTEM_SQLITE)

if(USE_SYSTEM_SQLITE)
  message(STATUS "  Using SQLite system version")
else()
  set(${proj}_DEPENDENCIES)
  set(SQLITE_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(SQLITE_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "https://sqlite.org/2014/sqlite-autoconf-3080702.tar.gz"
      URL_MD5 0f847048745ddbdf0c441c82d096fbb4
      BINARY_DIR ${SQLITE_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND ${SQLITE_SB_BUILD_DIR}/configure 
        --prefix=${CMAKE_INSTALL_PREFIX}
        --enable-static=no
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install
      DEPENDS ${${proj}_DEPENDENCIES}
      )
    
    ExternalProject_Add_Step(${proj} copy_source
      COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${SQLITE_SB_SRC} ${SQLITE_SB_BUILD_DIR}
      DEPENDEES patch update
      DEPENDERS configure
      )
  
endif()