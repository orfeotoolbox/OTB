# $Id$
#
# Build sample programs
#
set( SAMPLE_READ sample_read )
set( SAMPLE_WRITE sample_write )
set( SAMPLE_FILES sample_files )

include_directories(
    ../../include
)

add_executable( ${SAMPLE_READ} read.cpp )
add_executable( ${SAMPLE_WRITE} write.cpp )
add_executable( ${SAMPLE_FILES} files.cpp )

target_link_libraries( ${SAMPLE_READ} ${LIBLAS_LIB_NAME} )
target_link_libraries( ${SAMPLE_WRITE} ${LIBLAS_LIB_NAME} )
target_link_libraries( ${SAMPLE_FILES} ${LIBLAS_LIB_NAME} )
