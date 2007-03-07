# Commented since we choosed to build one single library to compile ossim
#SUBDIRS(base elevation fonts imaging libpolyclip matrix parallel projections support_data vec vpf_util plugin init)

FILE(GLOB_RECURSE ossim_base_SRCS "base/*.cpp" "base/*.c")
FILE(GLOB_RECURSE ossim_base_HDRS "base/*.h")

FILE(GLOB_RECURSE ossim_elevation_SRCS "elevation/*.cpp" "elevation/*.c")
FILE(GLOB_RECURSE ossim_elevation_HDRS "elevation/*.h")

FILE(GLOB_RECURSE ossim_fonts_SRCS "fonts/*.cpp" "fonts/*.c")
FILE(GLOB_RECURSE ossim_fonts_HDRS "fonts/*.h")

FILE(GLOB_RECURSE ossim_imaging_SRCS "imaging/*.cpp" "imaging/*.c")
FILE(GLOB_RECURSE ossim_imaging_HDRS "imaging/*.h")

FILE(GLOB_RECURSE ossim_libpolyclip_SRCS "libpolyclip/*.cpp" "libpolyclip/*.c")
FILE(GLOB_RECURSE ossim_libpolyclip_HDRS "libpolyclip/*.h")

FILE(GLOB_RECURSE ossim_matrix_SRCS "matrix/*.cpp" "matrix/*.c")
FILE(GLOB_RECURSE ossim_matrix_HDRS "matrix/*.h")

FILE(GLOB_RECURSE ossim_parallel_SRCS "parallel/*.cpp" "parallel/*.c")
FILE(GLOB_RECURSE ossim_parallel_HDRS "parallel/*.h")

FILE(GLOB_RECURSE ossim_projections_SRCS "projections/*.cpp" "projections/*.c")
FILE(GLOB_RECURSE ossim_projections_HDRS "projections/*.h")

FILE(GLOB_RECURSE ossim_support_data_SRCS "support_data/*.cpp" "support_data/*.c")
FILE(GLOB_RECURSE ossim_suport_data_HDRS "support_data/*.h")

FILE(GLOB_RECURSE ossim_vec_SRCS "vec/*.cpp" "vec/*.c")
FILE(GLOB_RECURSE ossim_vec_HDRS "vec/*.h")

FILE(GLOB_RECURSE ossim_vpf_util_SRCS "vpf_util/*.cpp" "vpf_util/*.c")
FILE(GLOB_RECURSE ossim_vpf_util_HDRS "vpf_util/*.h")

FILE(GLOB_RECURSE ossim_plugin_SRCS "plugin/*.cpp" "plugin/*.c")
FILE(GLOB_RECURSE ossim_plugin_HDRS "plugin/*.h")

FILE(GLOB_RECURSE ossim_init_SRCS "init/*.cpp" "init/*.c")
FILE(GLOB_RECURSE ossim_init_HDRS "init/*.h")

ADD_LIBRARY(ossim 
${ossim_base_SRCS}
${ossim_elevation_SRCS}
${ossim_fonts_SRCS}
${ossim_imaging_SRCS}
${ossim_libpolyclip_SRCS}
${ossim_matrix_SRCS}
${ossim_parallel_SRCS}
${ossim_projections_SRCS}
${ossim_support_data_SRCS}
${ossim_vec_SRCS}
${ossim_vpf_util_SRCS}
${ossim_plugin_SRCS}
${ossim_init_SRCS}
)

INSTALL_TARGETS(/lib/otb/ ossim)
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_base_HRDS})
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_elevation_HRDS})
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_fonts_HRDS})
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_imaging_HRDS})
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_libpolyclip_HRDS})
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_matrix_HRDS})
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_parallel_HRDS})
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_projections_HRDS})
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_support_data_HRDS})
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_vec_HRDS})
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_vpf_util_HRDS})
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_plugin_HRDS})
INSTALL_FILES(/include/otb/Utilities/OSSIM/ossim_core/ "\\.h$" ${ossim_init_HRDS})
