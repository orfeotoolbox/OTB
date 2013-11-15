set(EDISON_SOURCE
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

set(EDISON_HEADERS
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
if(CMAKE_COMPILER_IS_GNUCXX)
      set_source_files_properties(${EDISON_SOURCE} PROPERTIES COMPILE_FLAGS -w)
else()
  if(WIN32)
    if(CMAKE_CXX_COMPILER MATCHES "^cl$")
        set_source_files_properties( ${EDISON_SOURCE} PROPERTIES COMPILE_FLAGS "/W0" )
    endif()
  endif()
endif()


add_library(otbedison ${EDISON_SOURCE})
if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(otbedison PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif()

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS otbedison
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif()

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  install(FILES ${EDISON_HEADERS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbedison
    COMPONENT Development)
endif()
