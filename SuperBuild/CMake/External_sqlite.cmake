set(proj SQLITE)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup SQLite...")

if(USE_SYSTEM_SQLITE)
  # TODO
  #find_package ( SQLite )
  add_custom_target(${proj})
  message(STATUS "  Using SQLite system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using SQLite SuperBuild version")
  
  if(UNIX)
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "https://sqlite.org/2014/sqlite-autoconf-3080702.tar.gz"
      URL_MD5 0f847048745ddbdf0c441c82d096fbb4
      BINARY_DIR ${SQLITE_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      CONFIGURE_COMMAND ${SQLITE_SB_BUILD_DIR}/configure 
        --prefix=${SB_INSTALL_PREFIX}
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
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://www.sqlite.org/2015/sqlite-amalgamation-3080801.zip"
      URL_MD5 b1cbcbd710bdfd762dc169f1676053b5
      SOURCE_DIR ${SQLITE_SB_SRC}
      BINARY_DIR ${SQLITE_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt ${SQLITE_SB_SRC}
      UPDATE_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/patches/${proj}/sqlite3_win.c ${SQLITE_SB_SRC}
      CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_SHARED_LIBS:BOOL=ON      
      CMAKE_COMMAND
    )

  endif()
  
  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/sqlite3.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libsqlite3${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()
  
endif()
endif()
