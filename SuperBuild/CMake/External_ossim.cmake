set(proj OSSIM)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup Ossim...")

# OSGeo4W provides an "ossim" package : use it otherwise if it is installed and not used by OTB
# we get conflicts (because OSGeo4W include dir is included for other dependencies
#if(WIN32)
#  set(DEFAULT_USE_SYSTEM_OSSIM  ON)
#endif()

if(USE_SYSTEM_OSSIM)
  find_package ( Ossim )
  message(STATUS "  Using OSSIM system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using OSSIM SuperBuild version")

  # declare dependencies
  ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(${proj} TIFF GEOTIFF GEOS JPEG OPENTHREADS FREETYPE)

  INCLUDE_SUPERBUILD_DEPENDENCIES(${${proj}_DEPENDENCIES})
  # set proj back to its original value
  set(proj OSSIM)

  ADD_SUPERBUILD_CMAKE_VAR(TIFF_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(TIFF_LIBRARY)
  ADD_SUPERBUILD_CMAKE_VAR(GEOTIFF_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(GEOTIFF_LIBRARY)
  ADD_SUPERBUILD_CMAKE_VAR(GEOS_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(GEOS_LIBRARY)
  ADD_SUPERBUILD_CMAKE_VAR(JPEG_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(JPEG_LIBRARY)
  ADD_SUPERBUILD_CMAKE_VAR(OPENTHREADS_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(OPENTHREADS_LIBRARY)
  ADD_SUPERBUILD_CMAKE_VAR(FREETYPE_LIBRARY)

  list(APPEND OSSIM_SB_CONFIG "FREETYPE_INCLUDE:PATH=${FREETYPE_INCLUDE_DIR}")

  set(OSSIM_CXX_FLAGS  -D__STDC_CONSTANT_MACROS)

  if(MSVC)
    set(OSSIM_CXX_FLAGS /EHsc)
    list(APPEND OSSIM_SB_CONFIG "-GNMake\ Makefiles")
  endif()

  # archive version
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://download.osgeo.org/ossim/source/ossim-1.8.20/ossim-1.8.20-1.tar.gz"
    URL_MD5 19bd40f12d65c2e5b2963466dc838854
    BINARY_DIR ${OSSIM_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CONFIGURE_COMMAND ${SB_CMAKE_COMMAND} ${OSSIM_SB_SRC}/ossim
    -DProject_WC_REVISION:STRING=23537
    -DSUBVERSION_FOUND:BOOL=FALSE
    -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
    -DCMAKE_BUILD_TYPE:STRING=Release
    -DCMAKE_CXX_FLAGS:STRING=${OSSIM_CXX_FLAGS}
    -DBUILD_OSSIM_MPI_SUPPORT:BOOL=OFF
    -DBUILD_OSSIM_FREETYPE_SUPPORT:BOOL=ON
    -DBUILD_OSSIM_APPS:BOOL=OFF
    -DBUILD_OSSIM_TEST_APPS:BOOL=OFF
    -DBUILD_OSSIM_FRAMEWORKS:BOOL=OFF
    -DINSTALL_ARCHIVE_DIR:STRING=lib
    -DINSTALL_LIBRARY_DIR:STRING=lib
    -DCMAKE_MODULE_PATH:PATH=${OSSIM_SB_SRC}/ossim_package_support/cmake/CMakeModules
    -DCMAKE_PREFIX_PATH:STRING=${SB_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}
    ${OSSIM_SB_CONFIG}
    DEPENDS ${${proj}_DEPENDENCIES}
    )

  if(WIN32)
    if(MSVC)
      ExternalProject_Add_Step(${proj} patch_ossim_src
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/patches/ossim/src ${OSSIM_SB_SRC}/ossim/src
        DEPENDEES patch_ossim_include
        DEPENDERS configure )

      ExternalProject_Add_Step(${proj} patch_ossim_include
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/patches/ossim/include ${OSSIM_SB_SRC}/ossim/include
        DEPENDEES patch_ossim_cmake1
        DEPENDERS patch_ossim_src )
        
    endif()
    
    ExternalProject_Add_Step(${proj} patch_ossim_cmake1
      COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_SOURCE_DIR}/patches/ossim/CMakeLists.txt ${OSSIM_SB_SRC}/ossim/
      DEPENDEES patch_ossim_cmake2
      DEPENDERS patch_ossim_include )
      
    ExternalProject_Add_Step(${proj} patch_ossim_cmake2
      COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_SOURCE_DIR}/patches/ossim/OssimUtilities.cmake ${OSSIM_SB_SRC}/ossim_package_support/cmake/CMakeModules
      DEPENDEES patch
      DEPENDERS patch_ossim_cmake1 )      
  endif()

  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/ossim.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libossim${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()

endif()
