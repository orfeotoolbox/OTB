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

# Removing empty source file causing compilation warning on visual
REMOVE(ossim_support_data_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/support_data/ossimDemPoint.cpp")
REMOVE(ossim_plugin_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/plugin/ossimSharedObjectBridge.cpp")
REMOVE(ossim_base_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/base/ossimGzStream.cpp")
REMOVE(ossim_base_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/ossimFactoryBaseTemplate.cpp")

# Adjust the compiler flags to avoid problems with ossim code.
IF(CMAKE_COMPILER_IS_GNUCXX)
  FOREACH(f      ${ossim_base_SRCS} ${ossim_elevation_SRCS} ${ossim_font_SRCS} ${ossim_imaging_SRCS} ${ossim_matrix_SRCS} 
                 ${ossim_parallel_SRCS} ${ossim_projection_SRCS} ${ossim_support_data_SRCS}
                 ${ossim_vec_SRCS} ${ossim_vpfutil_SRCS} ${ossim_plugin_SRCS} ${ossim_init_SRCS} 
                 ${ossim_kbool_SRCS} ${ossim_dll_main_SRCS} )
        SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS -w )
  ENDFOREACH(f)
ELSE(CMAKE_COMPILER_IS_GNUCXX)
  IF(NOT BORLAND)
    IF(NOT CYGWIN)
      IF(NOT MINGW)
        FOREACH(f       ${ossim_base_SRCS} ${ossim_elevation_SRCS} ${ossim_font_SRCS} ${ossim_imaging_SRCS} ${ossim_matrix_SRCS} 
                        ${ossim_parallel_SRCS} ${ossim_projection_SRCS} ${ossim_support_data_SRCS}
                        ${ossim_vec_SRCS} ${ossim_vpfutil_SRCS} ${ossim_plugin_SRCS} ${ossim_init_SRCS} 
                        ${ossim_kbool_SRCS} ${ossim_dll_main_SRCS} )
                SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS /W0 )
        ENDFOREACH(f)
      ENDIF(NOT MINGW)
    ENDIF(NOT CYGWIN)
  ENDIF(NOT BORLAND)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(WIN32)
  IF(NOT BORLAND)
    IF(NOT CYGWIN)
      IF(NOT MINGW)
       SET(ossim_init_SRCS ${ossim_init_SRCS} ${ossim_dll_main_SRCS})
      ENDIF(NOT MINGW)
    ENDIF(NOT CYGWIN)
  ENDIF(NOT BORLAND)
ENDIF(WIN32)

INCLUDE_DIRECTORIES(${OTB_BINARY_DIR}/Utilities/otbgeotiff/)

#Specify that we are making DLL here
ADD_DEFINITIONS(-DNOMINMAX -DOSSIMMAKINGDLL)

ADD_LIBRARY(otbossim 
${ossim_base_SRCS}
${ossim_kbool_SRCS}
${ossim_matrix_SRCS}
${ossim_vec_SRCS}
${ossim_vpfutil_SRCS}
${ossim_plugin_SRCS}
${ossim_init_SRCS}
${ossim_font_SRCS}
${ossim_support_data_SRCS}
${ossim_projection_SRCS}
${ossim_imaging_SRCS} 
${ossim_parallel_SRCS} 
${ossim_elevation_SRCS}
)
TARGET_LINK_LIBRARIES(otbossim otbgeotiff otbxtiff itktiff itkjpeg8 itkjpeg12 itkjpeg16 OpenThreads)

INSTALL(TARGETS otbossim
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

