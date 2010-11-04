###############################################################################
#
# OSGeo4W packaging
#
###############################################################################

MESSAGE(STATUS "Building osgeo4w binaries")
set(OSGEO4W_DIR osgeo4w)
set(OSGEO4W_LIB_DIR ${OSGEO4W_DIR}/lib)
set(OSGEO4W_LIB_BIN_DIR ${OSGEO4W_DIR}/lib/bin)
set(OSGEO4W_DEVEL_DIR ${OSGEO4W_DIR}/devel)
set(OSGEO4W_DEVEL_INCLUDE_DIR ${OSGEO4W_DEVEL_DIR}/include)
set(OSGEO4W_DEVEL_INCLUDE_LIBLAS_DIR ${OSGEO4W_DEVEL_INCLUDE_DIR}/liblas)
set(OSGEO4W_DEVEL_LIB_DIR ${OSGEO4W_DEVEL_DIR}/lib)
set(OSGEO4W_DEVEL_BIN_DIR ${OSGEO4W_DEVEL_DIR}/bin)

set(OSGEO4W_PYTHON_DIR ${OSGEO4W_DIR}/apps/python25/lib/site-packages/liblas)
set(OSGEO4W_PACKAGES ${OSGEO4W_DIR}/packages)


set(OSGEO4W_DIRECTORIES
    ${OSGEO4W_DIR}
    ${OSGEO4W_LIB_DIR}
    ${OSGEO4W_LIB_BIN_DIR}
    ${OSGEO4W_DEVEL_DIR}
    ${OSGEO4W_DEVEL_INCLUDE_DIR}
    ${OSGEO4W_DEVEL_INCLUDE_LIBLAS_DIR}
    ${OSGEO4W_DEVEL_LIB_DIR}
    ${OSGEO4W_PYTHON_DIR}
    ${OSGEO4W_DEVEL_BIN_DIR}
    ${OSGEO4W_PACKAGES})



add_custom_target(make_osgeo4w_directories
  COMMAND ${CMAKE_COMMAND} -E echo "Building OSGeo4W install directories")

add_dependencies(  make_osgeo4w_directories las2las2  )

macro (make_directories)
    add_custom_command(
        TARGET make_osgeo4w_directories
        COMMAND ${CMAKE_COMMAND} -E  remove_directory  ${libLAS_SOURCE_DIR}/osgeo4w DEPENDS osgeo4w
    )
    foreach(directory ${OSGEO4W_DIRECTORIES})

    STRING(REGEX REPLACE "/" "_" target "${directory}" )

    add_custom_command(
        TARGET make_osgeo4w_directories
        COMMAND ${CMAKE_COMMAND} -E make_directory ${directory}
    )

    endforeach()

endmacro(make_directories)



add_custom_target(copy ALL COMMENT "Copying OSGeo4W files")
add_dependencies( copy make_osgeo4w_directories  )


macro(copy_files GLOBPAT DESTINATION  )
    
    MESSAGE(STATUS "   Copying ${GLOBPAT} to ${DESTINATION}")
    file(GLOB_RECURSE COPY_FILES
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        ${GLOBPAT})
    foreach(FILENAME ${COPY_FILES})
        set(SRC "${FILENAME}")
        set(DST "${DESTINATION}")
        add_custom_command(
            TARGET copy
            COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST}
        )
    endforeach(FILENAME)
endmacro(copy_files)


add_custom_target(tar
  COMMAND ${CMAKE_COMMAND} -E echo "Tarring OSGeo4W install")
add_dependencies( tar copy  )

macro (tar_directories source destination base_path)

    MESSAGE(STATUS "   Tarring ${source} to ${destination}")
    add_custom_command(
        TARGET tar
        COMMAND ${CMAKE_COMMAND} -E chdir ${source} cmake -E tar czf  ${destination} ${base_path}/
    )


endmacro(tar_directories)

make_directories()
copy_files(${LIBLAS_BUILD_OUTPUT_DIRECTORY}/las* ${OSGEO4W_DEVEL_BIN_DIR}/  )
copy_files(${LIBLAS_BUILD_OUTPUT_DIRECTORY}/*[.${SO_EXT}] ${OSGEO4W_DEVEL_BIN_DIR}/  )

copy_files(${LIBLAS_BUILD_OUTPUT_DIRECTORY}/*.lib ${OSGEO4W_DEVEL_LIB_DIR}/  )
copy_files(${LIBLAS_BUILD_OUTPUT_DIRECTORY}/*.a ${OSGEO4W_DEVEL_LIB_DIR}/  )

copy_files(./include/liblas/* ${OSGEO4W_DEVEL_INCLUDE_LIBLAS_DIR}/  )
copy_files(./python/liblas/*.py ${OSGEO4W_PYTHON_DIR}/  )

tar_directories(${OSGEO4W_DEVEL_DIR} ${libLAS_SOURCE_DIR}/${OSGEO4W_PACKAGES}/liblas-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}.tar.gz bin)
tar_directories(${OSGEO4W_DIR} ${libLAS_SOURCE_DIR}/${OSGEO4W_PACKAGES}/liblas-python-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}.tar.gz apps)
tar_directories(${OSGEO4W_DIR} ${libLAS_SOURCE_DIR}/${OSGEO4W_PACKAGES}/liblas-devel-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}.tar.gz devel)


add_custom_target(osgeo4w
    COMMAND ${CMAKE_COMMAND} -E echo "Making OSGeo4W build")
add_custom_command(
    TARGET osgeo4w
    COMMAND ${CMAKE_COMMAND} -E echo "Making OSGeo4W build"
    )
add_dependencies( osgeo4w tar   )


foreach(utility ${LIBLAS_UTILITIES})
    add_dependencies(  osgeo4w  ${utility} )

endforeach()


