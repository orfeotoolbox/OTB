# Commented since we choosed to build one single library to compile ossim
#SUBDIRS(base elevation fonts imaging libpolyclip matrix parallel projections support_data vec vpf_util plugin init)

FILE(GLOB_RECURSE ossim_base_SRCS "base/*.cpp" "base/*.c")
FILE(GLOB_RECURSE ossim_elevation_SRCS "elevation/*.cpp" "elevation/*.c")
FILE(GLOB_RECURSE ossim_font_SRCS "font/*.cpp" "font/*.c")
FILE(GLOB_RECURSE ossim_imaging_SRCS "imaging/*.cpp" "imaging/*.c")
#FILE(GLOB_RECURSE ossim_libpolyclip_SRCS "libpolyclip/*.cpp" "libpolyclip/*.c")
FILE(GLOB_RECURSE ossim_matrix_SRCS "matrix/*.cpp" "matrix/*.c")
FILE(GLOB_RECURSE ossim_parallel_SRCS "parallel/*.cpp" "parallel/*.c")
FILE(GLOB_RECURSE ossim_projection_SRCS "projection/*.cpp" "projection/*.c")
FILE(GLOB_RECURSE ossim_support_data_SRCS "support_data/*.cpp" "support_data/*.c")
FILE(GLOB_RECURSE ossim_vec_SRCS "vec/*.cpp" "vec/*.c")
FILE(GLOB_RECURSE ossim_vpfutil_SRCS "vpfutil/*.cpp" "vpfutil/*.c")
FILE(GLOB_RECURSE ossim_plugin_SRCS "plugin/*.cpp" "plugin/*.c")
FILE(GLOB_RECURSE ossim_init_SRCS "init/*.cpp" "init/*.c")
FILE(GLOB_RECURSE ossim_kbool_SRCS "kbool/*.cpp" "kbool/*.c")
FILE(GLOB_RECURSE ossim_dll_main_SRCS "dll_main/*.cpp")

SET(OSSIM_SRCS
${ossim_base_SRCS}
${ossim_elevation_SRCS}
${ossim_font_SRCS}
${ossim_imaging_SRCS}
${ossim_matrix_SRCS}
${ossim_parallel_SRCS}
${ossim_projection_SRCS}
${ossim_support_data_SRCS}
${ossim_vec_SRCS}
${ossim_vpfutil_SRCS}
${ossim_plugin_SRCS}
${ossim_init_SRCS}
${ossim_kbool_SRCS}
)


IF(WIN32)
  IF(NOT BORLAND)
    IF(NOT CYGWIN)
      IF(NOT MINGW)
 	SET(OSSIM_SRCS ${OSSIM_SRCS} ${ossim_dll_main_SRCS})          
      ENDIF(NOT MINGW)
    ENDIF(NOT CYGWIN)
  ENDIF(NOT BORLAND)
ENDIF(WIN32)


ADD_LIBRARY(ossim ${OSSIM_SRCS})
TARGET_LINK_LIBRARIES(ossim itktiff itkjpeg8 itkjpeg12 itkjpeg16)
INSTALL_TARGETS(/lib/otb/ ossim)
INSTALL_FILES(/include/otb/Utilities/OSSIM/include/ "\\.h$" ${ossim_kbool_HRDS})
