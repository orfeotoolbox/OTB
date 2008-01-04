FILE(GLOB_RECURSE ossim_base_SRCS "base/*.cpp" "base/*.c")
FILE(GLOB_RECURSE ossim_elevation_SRCS "elevation/*.cpp" "elevation/*.c")
FILE(GLOB_RECURSE ossim_font_SRCS "font/*.cpp" "font/*.c")
FILE(GLOB_RECURSE ossim_imaging_SRCS "imaging/*.cpp" "imaging/*.c")
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

# Adjust the compiler flags to avoid problems with ossim code.
IF(CMAKE_COMPILER_IS_GNUCXX)
  FOREACH(f      ${ossim_base_SRCS} ${ossim_elevation_SRCS} ${ossim_font_SRCS} ${ossim_imaging_SRCS} ${ossim_matrix_SRCS} 
                 ${ossim_parallel_SRCS} ${ossim_projection_SRCS} ${ossim_support_data_SRCS}
                 ${ossim_vec_SRCS} ${ossim_vpfutil_SRCS} ${ossim_plugin_SRCS} ${ossim_init_SRCS} 
                 ${ossim_kbool_SRCS} ${ossim_dll_main_SRCS} )
        SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS -w )
  ENDFOREACH(f)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

INCLUDE_DIRECTORIES(${OTB_BINARY_DIR}/Utilities/otbgeotiff/)

ADD_LIBRARY(otbossimKBool ${ossim_kbool_SRCS})

ADD_LIBRARY(otbossimMatrix ${ossim_matrix_SRCS})
TARGET_LINK_LIBRARIES(otbossimMatrix otbossimKBool)

ADD_LIBRARY(otbossimBase ${ossim_base_SRCS})
TARGET_LINK_LIBRARIES(otbossimBase otbossimMatrix)

ADD_LIBRARY(otbossimPlugin ${ossim_plugin_SRCS})
TARGET_LINK_LIBRARIES(otbossimPlugin otbossimBase)

ADD_LIBRARY(otbossimFont ${ossim_font_SRCS})
TARGET_LINK_LIBRARIES(otbossimFont otbossimPlugin)

ADD_LIBRARY(otbossimVpfUtil ${ossim_vpfutil_SRCS})
TARGET_LINK_LIBRARIES(otbossimVpfUtil otbossimFont)

ADD_LIBRARY(otbossimVec ${ossim_vec_SRCS})
TARGET_LINK_LIBRARIES(otbossimVec otbossimVpfUtil)

ADD_LIBRARY(otbossimSupportData ${ossim_support_data_SRCS})
TARGET_LINK_LIBRARIES(otbossimSupportData otbossimVec otbgeotiff otbxtiff)

ADD_LIBRARY(otbossimElevation ${ossim_elevation_SRCS})
TARGET_LINK_LIBRARIES(otbossimElevation otbossimSupportData)

#ADD_LIBRARY(otbossimProjection ${ossim_projection_SRCS})
#TARGET_LINK_LIBRARIES(otbossimProjection otbossimElevation)

#ADD_LIBRARY(otbossimImaging ${ossim_imaging_SRCS} ${ossim_parallel_SRCS})
#TARGET_LINK_LIBRARIES(otbossimImaging otbossimProjection itktiff itkjpeg8 itkjpeg12 itkjpeg16)

#################ADD_LIBRARY(otbossimProjectionImaging ${ossim_imaging_SRCS} ${ossim_projection_SRCS} ${ossim_parallel_SRCS})
#################TARGET_LINK_LIBRARIES(otbossimProjectionImaging otbossimElevation itktiff itkjpeg8 itkjpeg12 itkjpeg16)
################# CYRILLE MODIF POUR ESSAYER DE FAIRE MARCHE LE BOUSIN !!! #################
#ADD_LIBRARY(otbossimProjectionImaging ${ossim_imaging_SRCS} ${ossim_projection_SRCS} ${ossim_parallel_SRCS} ${ossim_base_SRCS})
#TARGET_LINK_LIBRARIES(otbossimProjectionImaging otbossimElevation itktiff itkjpeg8 itkjpeg12 itkjpeg16)
ADD_LIBRARY(otbossimProjectionImaging ${ossim_imaging_SRCS} ${ossim_projection_SRCS} ${ossim_parallel_SRCS})
TARGET_LINK_LIBRARIES(otbossimProjectionImaging otbossimElevation itktiff itkjpeg8 itkjpeg12 itkjpeg16)

IF(WIN32)
  IF(NOT BORLAND)
    IF(NOT CYGWIN)
      IF(NOT MINGW)
       SET(ossim_init_SRCS ${ossim_init_SRCS} ${ossim_dll_main_SRCS})
      ENDIF(NOT MINGW)
    ENDIF(NOT CYGWIN)
  ENDIF(NOT BORLAND)
ENDIF(WIN32)

ADD_LIBRARY(otbossim ${ossim_init_SRCS})
TARGET_LINK_LIBRARIES(otbossim otbossimProjectionImaging)
#TARGET_LINK_LIBRARIES(ossim ossimImaging)

INSTALL(TARGETS otbossimKBool
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

INSTALL(TARGETS otbossimMatrix
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

INSTALL(TARGETS otbossimBase
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

INSTALL(TARGETS otbossimPlugin
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

INSTALL(TARGETS otbossimFont
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

INSTALL(TARGETS otbossimVpfUtil
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

INSTALL(TARGETS otbossimVec
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

INSTALL(TARGETS otbossimSupportData
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

INSTALL(TARGETS otbossimElevation
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

#INSTALL(TARGETS otbossimProjection
#RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
#LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
#ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

#INSTALL(TARGETS otbossimImaging
#RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
#LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
#ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

INSTALL(TARGETS otbossimProjectionImaging
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

INSTALL(TARGETS otbossim
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
