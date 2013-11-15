project(otbossim)

set(OSSIM_MAJOR_VERSION_NUMBER "1")
set(OSSIM_MINOR_VERSION_NUMBER "8")
set(OSSIM_PATCH_VERSION_NUMBER "10")
set(OSSIM_RELEASE_NUMBER "?")
set(OSSIM_VERSION "${OSSIM_MAJOR_VERSION_NUMBER}-${OSSIM_MINOR_VERSION_NUMBER}-${OSSIM_PATCH_VERSION_NUMBER}" )

# svn revision number of this ossim fork (used for .so versioning)
set(OSSIM_SVN_REVISION "21971")
# patch number for OTB modifications applied on top of OSSIM_SVN_REVISION (used for .so versioning)
set(OSSIM_OTB_PATCH "1")

include(CheckIncludeFile)
CHECK_INCLUDE_FILE("dirent.h" CMAKE_HAVE_DIRENT_H)
CHECK_INCLUDE_FILE("unistd.h" CMAKE_HAVE_UNISTD_H)
CHECK_INCLUDE_FILE("getopt.h" CMAKE_HAVE_GETOPT_H)
CHECK_INCLUDE_FILE("fcntl.h" CMAKE_HAVE_FCNTL_H)
CHECK_INCLUDE_FILE("dbmalloc.h" CMAKE_HAVE_DBMALLOC_H)
CHECK_INCLUDE_FILE("malloc.h" CMAKE_HAVE_MALLOC_H)
CHECK_INCLUDE_FILE("dlfcn.h" CMAKE_HAVE_DLFCN_H)

include_directories(${GEOTIFF_INCLUDE_DIRS})
include_directories(${TIFF_INCLUDE_DIRS})

include_directories(${OTB_SOURCE_DIR}/Utilities/otbossim/include)
include_directories(${OTB_BINARY_DIR}/Utilities/otbossim/include)

configure_file(${OTB_SOURCE_DIR}/Utilities/otbossim/include/ossim/ossimConfig.h.in
               ${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimConfig.h)

  file(GLOB_RECURSE ossim_init_SRCS "src/ossim/init/*.cpp" "src/ossim/init/*.c")
  file(GLOB_RECURSE ossim_base_SRCS "src/ossim/base/*.cpp" "src/ossim/base/*.c")
  file(GLOB_RECURSE ossim_elevation_SRCS "src/ossim/elevation/*.cpp" "src/ossim/elevation/*.c")
  file(GLOB_RECURSE ossim_font_SRCS "src/ossim/font/*.cpp" "src/ossim/font/*.c")
  file(GLOB_RECURSE ossim_imaging_SRCS "src/ossim/imaging/*.cpp" "src/ossim/imaging/*.c")
  file(GLOB_RECURSE ossim_matrix_SRCS "src/ossim/matrix/*.cpp" "src/ossim/matrix/*.c")
  file(GLOB_RECURSE ossim_parallel_SRCS "src/ossim/parallel/*.cpp" "src/ossim/parallel/*.c")
  file(GLOB_RECURSE ossim_projection_SRCS "src/ossim/projection/*.cpp" "src/ossim/projection/*.c")
  file(GLOB_RECURSE ossim_support_data_SRCS "src/ossim/support_data/*.cpp" "src/ossim/support_data/*.c")
  file(GLOB_RECURSE ossim_util_SRCS "src/ossim/util/*.cpp" "src/ossim/util/*.c")
  file(GLOB_RECURSE ossim_vec_SRCS "src/ossim/vec/*.cpp" "src/ossim/vec/*.c")
  file(GLOB_RECURSE ossim_vpfutil_SRCS "src/ossim/vpfutil/*.cpp" "src/ossim/vpfutil/*.c")
  file(GLOB_RECURSE ossim_plugin_SRCS "src/ossim/plugin/*.cpp" "src/ossim/plugin/*.c")
  file(GLOB_RECURSE ossim_kbool_SRCS "src/ossim/kbool/*.cpp" "src/ossim/kbool/*.c")
  file(GLOB_RECURSE ossim_dll_main_SRCS "src/ossim/dll_main/*.cpp")

  # Removing empty source file causing compilation warning on visual
  list(REMOVE_ITEM ossim_support_data_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/ossim/support_data/ossimDemPoint.cpp")
  list(REMOVE_ITEM ossim_plugin_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/ossim/plugin/ossimSharedObjectBridge.cpp")
  list(REMOVE_ITEM ossim_base_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/ossim/base/ossimFactoryBaseTemplate.cpp")

  # Removing sources not in use any more (switched to ossim plugins)
  list(REMOVE_ITEM ossim_projection_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/ossim/projection/ossimCosmoSkymedModel.cpp")


  if(NOT OTB_BUILD_SHARED_LIB)
    if(WIN32)
      if(NOT CYGWIN)
        if(NOT MINGW)
        set(ossim_init_SRCS ${ossim_init_SRCS} ${ossim_dll_main_SRCS})
        endif(NOT MINGW)
      endif(NOT CYGWIN)
    endif(WIN32)
  endif(NOT OTB_BUILD_SHARED_LIB)


  add_executable(version-config src/ossim/version-config.cpp)

  add_custom_command(
          OUTPUT ${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimVersion.h
          COMMAND version-config
          ARGS "${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimVersion.h" "${OSSIM_VERSION}"
          DEPENDS version-config
          COMMENT "Generating ossimVersion.h" )

  set_source_files_properties(
      src/ossim/init/ossimInit.cpp PROPERTIES
      OBJECT_DEPENDS "${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimVersion.h"
      )

  #Specify that we are making DLL here
  if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DNOMINMAX -DOSSIMMAKINGDLL)
  endif(NOT BUILD_SHARED_LIBS)

  add_library(otbossim
  ${ossim_init_SRCS}
  ${ossim_base_SRCS}
  ${ossim_kbool_SRCS}
  ${ossim_matrix_SRCS}
  ${ossim_util_SRCS}
  ${ossim_vec_SRCS}
  ${ossim_vpfutil_SRCS}
  ${ossim_plugin_SRCS}
  ${ossim_font_SRCS}
  ${ossim_support_data_SRCS}
  ${ossim_projection_SRCS}
  ${ossim_imaging_SRCS}
  ${ossim_parallel_SRCS}
  ${ossim_elevation_SRCS})

  #Apparently not supported by opensolaris at least
#  set_target_properties(otbossim
#    PROPERTIES
#    LINK_INTERFACE_LIBRARIES ""
#  )
  #target_link_libraries(otbossim ${GDAL_LIBRARY})#To make sure that gdal appear before geotiff
  target_link_libraries(otbossim  ${GEOTIFF_LIBRARY} ${TIFF_LIBRARY} ${JPEG_LIBRARY} ${GDAL_LIBRARY} ${OPENTHREADS_LIBRARY})
  set_target_properties(otbossim
                        PROPERTIES VERSION "${OSSIM_SVN_REVISION}.${OSSIM_OTB_PATCH}.0"
                                   SOVERSION "${OSSIM_SVN_REVISION}.${OSSIM_OTB_PATCH}")

  if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS otbossim
          RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
          LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
          ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
  endif(NOT OTB_INSTALL_NO_LIBRARIES)

