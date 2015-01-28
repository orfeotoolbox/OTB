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
  
  if(UNIX)
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
  else(MSVC)
  
    ExternalProject_Add(${proj}_def
      PREFIX ${proj}/_def
      URL "http://www.sqlite.org/2015/sqlite-dll-win32-x86-3080801.zip"
      URL_MD5 0111362e9260805a90c26d60778cca60
      SOURCE_DIR ${SQLITE_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
      )
  
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://www.sqlite.org/2015/sqlite-amalgamation-3080801.zip"
      URL_MD5 b1cbcbd710bdfd762dc169f1676053b5
      BINARY_DIR ${SQLITE_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      PATCH_COMMAND  cl ${SQLITE_SB_SRC}/sqlite3.c /DSQLITE_ENABLE_COLUMN_METADATA=1 /DSQLITE_ENABLE_RTREE=1 -link -dll -out:${SQLITE_SB_BUILD_DIR}/sqlite3.dll /DEF:${SQLITE_SB_BUILD_DIR}/sqlite3.def
      UPDATE_COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE_SB_BUILD_DIR}/sqlite3.dll ${CMAKE_INSTALL_PREFIX}/bin/
      CONFIGURE_COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE_SB_BUILD_DIR}/sqlite3.lib ${CMAKE_INSTALL_PREFIX}/lib/
      BUILD_COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE_SB_SRC}/sqlite3.h ${CMAKE_INSTALL_PREFIX}/include/
      INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE_SB_SRC}/sqlite3ext.h ${CMAKE_INSTALL_PREFIX}/include/
      DEPENDS ${proj}_def
      )

  endif()
  
  
  message(STATUS "  Using SQLite SuperBuild version")
endif()