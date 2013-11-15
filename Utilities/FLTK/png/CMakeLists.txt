project(FLTKPNG)
include_regular_expression("^png.*$")

include_directories(${FLTKZLIB_SOURCE_DIR})

if(ZLIB_INCLUDE_DIR)
  include_directories(${ZLIB_INCLUDE_DIR})
endif(ZLIB_INCLUDE_DIR)

include_directories(${FLTKPNG_SOURCE_DIR})
include_directories(${FLTKPNG_BINARY_DIR})

# source files for png
set(PNG_SRCS
  pngget.c    pngrio.c    pngwrite.c
  png.c       pngmem.c    pngrtran.c  pngtrans.c  pngwtran.c
  pngerror.c  pngpread.c  pngrutil.c  pngwutil.c
  pngread.c   pngset.c   pngwio.c
  )

add_library(fltk_png ${PNG_SRCS})
install(TARGETS fltk_png
        RUNTIME DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
        LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
        ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)


if(UNIX)
  target_link_libraries(fltk_png ${FLTK_ZLIB_LIBRARIES} -lm)
else(UNIX)
  target_link_libraries(fltk_png ${FLTK_ZLIB_LIBRARIES})
endif(UNIX)

