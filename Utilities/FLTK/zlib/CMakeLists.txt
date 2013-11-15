project(FLTKZLIB)
include_regular_expression("^(deflate|inf|trees|zconf|zlib|zutil).*$")

include_directories(${FLTKZLIB_SOURCE_DIR})
include_directories(${FLTKZLIB_BINARY_DIR})

# source files for zlib
set(ZLIB_SRCS
adler32.c compress.c crc32.c deflate.c gzio.c inffast.c
inflate.c inftrees.c trees.c uncompr.c zutil.c
)

add_library(fltk_zlib ${ZLIB_SRCS})
install(TARGETS fltk_zlib
        RUNTIME DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
        LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
        ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)


