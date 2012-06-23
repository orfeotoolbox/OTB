SET(EDISON_SOURCE
prompt/parser.cpp 
prompt/globalFnc.cpp 
prompt/edison.cpp
prompt/libppm.cpp
segm/msSysPrompt.cpp 
segm/ms.cpp
segm/msImageProcessor.cpp
segm/RAList.cpp
segm/rlist.cpp
edge/BgEdge.cpp
edge/BgEdgeDetect.cpp
edge/BgEdgeList.cpp
edge/BgGlobalFc.cpp
edge/BgImage.cpp)

SET(EDISON_HEADERS
prompt/parser.h 
prompt/defs.h 
prompt/error.h 
prompt/flags.h 
prompt/edison.h
prompt/imfilt.h 
prompt/libppm.h
segm/tdef.h
segm/RAList.h
segm/ms.h
segm/msImageProcessor.h 
segm/msSys.h
segm/rlist.h
edge/BgDefaults.h
edge/BgEdge.h
edge/BgEdgeDetect.h
edge/BgImage.h
)

# Suppress deprecated warning
IF(CMAKE_COMPILER_IS_GNUCXX)
      SET_SOURCE_FILES_PROPERTIES(${EDISON_SOURCE} PROPERTIES COMPILE_FLAGS -w)
ELSE(CMAKE_COMPILER_IS_GNUCXX)
  IF(WIN32)
    IF (CMAKE_CXX_COMPILER MATCHES "^cl$")
        SET_SOURCE_FILES_PROPERTIES( ${EDISON_SOURCE} PROPERTIES COMPILE_FLAGS "/W0" )
    ENDIF (CMAKE_CXX_COMPILER MATCHES "^cl$")
  ENDIF(WIN32)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)


ADD_LIBRARY(otbedison ${EDISON_SOURCE})
IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(otbedison PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)

IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS otbedison
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES ${EDISON_HEADERS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbedison
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)
