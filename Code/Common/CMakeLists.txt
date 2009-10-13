# Sources of non-templated classes.

FILE(GLOB OTBCommon_SRCS "*.cxx" )

IF( NOT OTB_USE_MAPNIK )
    LIST(REMOVE_ITEM OTBCommon_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/otbVectorDataStyle.cxx" )
ENDIF( NOT OTB_USE_MAPNIK )

IF( NOT OTB_USE_PQXX )
    LIST(REMOVE_ITEM OTBCommon_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/otbPostGISConnectionImplementation.cxx" )
ENDIF( NOT OTB_USE_PQXX )


ADD_LIBRARY(OTBCommon ${OTBCommon_SRCS})
TARGET_LINK_LIBRARIES (OTBCommon ITKAlgorithms ITKStatistics ITKCommon otbconfigfile)
IF (OTB_USE_MAPNIK)
    TARGET_LINK_LIBRARIES(OTBCommon ${MAPNIK_LIBRARY})
ENDIF(OTB_USE_MAPNIK)
IF (OTB_USE_PQXX)
#TODO this line should be refined when we will like to have this capability with windows
    TARGET_LINK_LIBRARIES(OTBCommon pq pqxx)
ENDIF(OTB_USE_PQXX)


IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(OTBCommon PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)

FILE(GLOB otbconfigfileCreation_SRCS "${OTB_SOURCE_DIR}/CMake/generate-config-properties.cpp")
SET(otbconfigfile_DEFAULT "${OTB_BINARY_DIR}/otb_config.inp")

ADD_EXECUTABLE(generate-config-properties ${otbconfigfileCreation_SRCS})

ADD_CUSTOM_COMMAND(
          TARGET OTBCommon
          POST_BUILD
          COMMAND generate-config-properties
          ARGS "${otbconfigfile_DEFAULT}" "${OTB_LANG}"
          COMMENT "Generating otb_config.inp" )

IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS OTBCommon
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  INSTALL(FILES ${__files1} ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Common
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)
