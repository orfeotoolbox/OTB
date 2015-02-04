cmake_minimum_required(VERSION 2.8.3)

project(libjpeg)

#jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h jversion.h jmemsys.h
set(LIBJPEG_SRCS jcapimin.c jcapistd.c jcarith.c jctrans.c jcparam.c jcinit.c jcmainct.c jcmarker.c jcmaster.c jdatadst.c jccoefct.c jccolor.c jcprepct.c jcsample.c jcdctmgr.c jchuff.c jfdctflt.c jfdctfst.c jfdctint.c jdapimin.c jdapistd.c jdarith.c jdtrans.c jdatasrc.c jdmaster.c jdinput.c jdmarker.c jdhuff.c jdmainct.c jdcoefct.c jdpostct.c jddctmgr.c jidctfst.c jidctflt.c jidctint.c jdsample.c jdcolor.c jquant1.c jquant2.c jdmerge.c jaricom.c jcomapi.c jerror.c jutils.c jmemnobs.c jmemmgr.c)

set(LIBJPEG_INCLUDES jdct.h jconfig.h jerror.h jinclude.h jmemsys.h jmorecfg.h jpegint.h jpeglib.h jversion.h)

include_directories("${CMAKE_SOURCE_DIR}")

add_library(libjpeg ${LIBJPEG_SRCS})

install(TARGETS libjpeg
  RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
  LIBRARY DESTINATION lib COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION lib COMPONENT Development)


  
foreach(LIBJPEG_INCLUDE ${LIBJPEG_INCLUDES})
  install(FILES ${LIBJPEG_INCLUDE}  DESTINATION include  COMPONENT Development)
endforeach()

