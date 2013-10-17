SET(LIB_NAME ossim)

########################################### BEGIN: GRAB SOURCE FILES ##########################################
FILE(GLOB ossim_dll_main_SRCS RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/dll_main/*.cpp")
FILE(GLOB ossim_SRCS  RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} 
        "${CMAKE_CURRENT_SOURCE_DIR}/ossimConfig*.cpp" 
        "${CMAKE_CURRENT_SOURCE_DIR}/ossimVer*.cpp" 
        "${CMAKE_CURRENT_SOURCE_DIR}/init/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/init/*.c" 
        "${CMAKE_CURRENT_SOURCE_DIR}/base/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/base/*.c" 
        "${CMAKE_CURRENT_SOURCE_DIR}/elevation/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/elevation/*.c" 
        "${CMAKE_CURRENT_SOURCE_DIR}/font/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/font/*.c" 
        "${CMAKE_CURRENT_SOURCE_DIR}/imaging/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/imaging/*.c" 
        "${CMAKE_CURRENT_SOURCE_DIR}/matrix/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/matrix/*.c" 
        "${CMAKE_CURRENT_SOURCE_DIR}/parallel/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/parallel/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/projection/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/projection/*.c" 
        "${CMAKE_CURRENT_SOURCE_DIR}/support_data/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/support_data/*.c" 
        "${CMAKE_CURRENT_SOURCE_DIR}/util/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/util/*.c" 
        "${CMAKE_CURRENT_SOURCE_DIR}/vec/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/vec/*.c" 
        "${CMAKE_CURRENT_SOURCE_DIR}/video/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/video/*.c" 
        "${CMAKE_CURRENT_SOURCE_DIR}/vpfutil/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/vpfutil/*.c" 
        "${CMAKE_CURRENT_SOURCE_DIR}/plugin/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/plugin/*.c")

IF(WIN32)
	LIST(APPEND ossim_SRCS "${ossim_dll_main_SRCS}")
ENDIF(WIN32)

ADD_DEFINITIONS("-DOSSIMMAKINGDLL")
############################### GRAB HEADERS #####################################
FILE(GLOB ossim_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/*.h")
FILE(GLOB ossim_base_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/base/*.h")
FILE(GLOB ossim_elevation_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/elevation/*.h")
FILE(GLOB ossim_font_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/font/*.h")
FILE(GLOB ossim_imaging_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/imaging/*.h")
FILE(GLOB ossim_init_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/init/*.h")
FILE(GLOB ossim_matrix_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/matrix/*.h")
FILE(GLOB ossim_parallel_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/parallel/*.h")
FILE(GLOB ossim_plugin_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/plugin/*.h")
FILE(GLOB ossim_projection_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/projection/*.h")
FILE(GLOB ossim_support_data_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/support_data/*.h")
FILE(GLOB ossim_util_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/util/*.h")
FILE(GLOB ossim_vec_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/vec/*.h")
FILE(GLOB ossim_video_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/video/*.h")
FILE(GLOB ossim_vpfutil_HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/ossim/vpfutil/*.h")

SET(OSSIM_HEADER_FILES 
	${ossim_HDRS}
 	${ossim_base_HDRS} ${ossim_elevation_HDRS} ${ossim_font_HDRS}  ${ossim_imaging_HDRS} ${ossim_init_HDRS}
	${ossim_matrix_HDRS} ${ossim_parallel_HDRS} ${ossim_plugin_HDRS} ${ossim_projection_HDRS} ${ossim_support_data_HDRS} ${ossim_util_HDRS} ${ossim_vec_HDRS} ${ossim_video_HDRS} ${ossim_vpfutil_HDRS} )


################################## APPLE Framework support deep header mapping ###################################
IF(APPLE)
  FOREACH(f  ${ossim_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers )
  ENDFOREACH(f)
  FOREACH(f  ${ossim_base_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/base )
  ENDFOREACH(f)
  FOREACH(f  ${ossim_elevation_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/elevation )
  ENDFOREACH(f)
  FOREACH(f  ${ossim_font_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/font )
  ENDFOREACH(f)
  FOREACH(f  ${ossim_imaging_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/imaging )
  ENDFOREACH(f)
  FOREACH(f  ${ossim_init_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/init )
  ENDFOREACH(f)
  FOREACH(f  ${ossim_matrix_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/matrix )
  ENDFOREACH(f)
  FOREACH(f  ${ossim_parallel_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/parallel )
  ENDFOREACH(f)
  FOREACH(f  ${ossim_plugin_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/plugin )
  ENDFOREACH(f)
  FOREACH(f  ${ossim_projection_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/projection )
  ENDFOREACH(f)
  FOREACH(f  ${ossim_support_data_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/support_data )
  ENDFOREACH(f)
  FOREACH(f  ${ossim_util_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/util )
  ENDFOREACH(f)
  FOREACH(f  ${ossim_vec_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/vec )
  ENDFOREACH(f)
  FOREACH(f  ${ossim_video_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/video )
  ENDFOREACH(f)

  FOREACH(f  ${ossim_vpfutil_HDRS} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/vpfutil )
  ENDFOREACH(f)

ENDIF(APPLE)

#################################### SETUP the required link parameters using the Ossim macro ####################################
OSSIM_LINK_LIBRARY(${LIB_NAME} 
                   COMPONENT_NAME ossim 
                   TYPE ${OSSIM_USER_DEFINED_DYNAMIC_OR_STATIC}
                   LIBRARIES ${ossimDependentLibs}
#                   HEADERS "${OSSIM_HEADER_FILES}" 
#                   PUBLIC_HEADERS "${ossim_HDRS}"
                   SOURCE_FILES "${ossim_SRCS}"
                   INSTALL_LIB VERSION_SYMLINKS INSTALL_HEADERS)

########################################################## If DO special case for ossim core needs to have the files mapped properly ################################ 
IF(NOT OSSIM_BUILD_FRAMEWORKS OR NOT APPLE)
   install(FILES ${ossim_HDRS} DESTINATION include/ossim COMPONENT ossim) 
   install(FILES ${ossim_base_HDRS} DESTINATION include/ossim/base COMPONENT ossim) 
   install(FILES ${ossim_elevation_HDRS} DESTINATION include/ossim/elevation COMPONENT ossim)
   install(FILES ${ossim_font_HDRS} DESTINATION include/ossim/font COMPONENT ossim)
   install(FILES ${ossim_imaging_HDRS} DESTINATION include/ossim/imaging COMPONENT ossim) 
   install(FILES ${ossim_init_HDRS} DESTINATION include/ossim/init COMPONENT ossim) 
   install(FILES ${ossim_matrix_HDRS} DESTINATION include/ossim/matrix COMPONENT ossim) 
   install(FILES ${ossim_parallel_HDRS} DESTINATION include/ossim/parallel COMPONENT ossim) 
   install(FILES ${ossim_plugin_HDRS} DESTINATION include/ossim/plugin COMPONENT ossim) 
   install(FILES ${ossim_projection_HDRS} DESTINATION include/ossim/projection COMPONENT ossim) 
   install(FILES ${ossim_support_data_HDRS} DESTINATION include/ossim/support_data COMPONENT ossim) 
   install(FILES ${ossim_util_HDRS} DESTINATION include/ossim/util COMPONENT ossim) 
   install(FILES ${ossim_vec_HDRS} DESTINATION include/ossim/vec COMPONENT ossim) 
   install(FILES ${ossim_video_HDRS} DESTINATION include/ossim/video COMPONENT ossim) 
   install(FILES ${ossim_vpfutil_HDRS} DESTINATION include/ossim/vpfutil COMPONENT ossim) 
ENDIF(NOT OSSIM_BUILD_FRAMEWORKS OR NOT APPLE)
# ENDIF(NOT OSSIM_FRAMEWORK_GENERATION OR NOT APPLE)

# ---
# This is the old list of library for the link line.  Replaced with ${ossimDependentLibs}.  
# Kept temporarily in case I broke something.  drb - 31 Aug. 2011
# LIBRARIES ${JPEG_LIBRARIES} ${TIFF_LIBRARIES} ${GEOTIFF_LIBRARIES} ${OPENTHREADS_LIBRARIES} ${FREETYPE_LIBRARIES} ${MPI_LIBRARIES} ${ZLIB_LIBRARIES} ${DL_LIBRARY}
# ---
