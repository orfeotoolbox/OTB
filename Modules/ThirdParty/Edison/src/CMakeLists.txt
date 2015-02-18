set(OTB3P_INSTALL_EXPORT_NAME "${OTBEdison-targets}")
set(OTB3P_INSTALL_INCLUDE_DIR "${OTBEdison_INSTALL_INCLUDE_DIR}")
set(OTB3P_INSTALL_RUNTIME_DIR "${OTBEdison_INSTALL_RUNTIME_DIR}")
set(OTB3P_INSTALL_LIBRARY_DIR "${OTBEdison_INSTALL_LIBRARY_DIR}")
set(OTB3P_INSTALL_ARCHIVE_DIR "${OTBEdison_INSTALL_ARCHIVE_DIR}")

set(EDISON_SOURCES
#  prompt/parser.cpp
#  prompt/globalFnc.cpp
#  prompt/edison.cpp
#  prompt/libppm.cpp
  otbedison/msSysPrompt.cpp
  otbedison/ms.cpp
  otbedison/msImageProcessor.cpp
  otbedison/RAList.cpp
  otbedison/rlist.cpp
#  edge/BgEdge.cpp
#  edge/BgEdgeDetect.cpp
#  edge/BgEdgeList.cpp
#  edge/BgGlobalFc.cpp
#  edge/BgImage.cpp
  )

set(EDISON_HEADERS
#  prompt/parser.h
#  prompt/defs.h
#  prompt/error.h
#  prompt/flags.h
#  prompt/edison.h
#  prompt/imfilt.h
#  prompt/libppm.h
  otbedison/tdef.h
  otbedison/RAList.h
  otbedison/ms.h
  otbedison/msImageProcessor.h
  otbedison/msSys.h
  otbedison/rlist.h
#  edge/BgDefaults.h
#  edge/BgEdge.h
#  edge/BgEdgeDetect.h
#  edge/BgImage.h
  )

add_library(otbedison ${EDISON_SOURCES})

# Apply user-defined properties to the library target.
if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(otbedison PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif(OTB_LIBRARY_PROPERTIES)

install(TARGETS otbedison
  EXPORT ${OTB3P_INSTALL_EXPORT_NAME}
  RUNTIME DESTINATION ${OTB3P_INSTALL_RUNTIME_DIR} COMPONENT RuntimeLibraries
  LIBRARY DESTINATION ${OTB3P_INSTALL_LIBRARY_DIR} COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION ${OTB3P_INSTALL_ARCHIVE_DIR} COMPONENT Development
  )

# Forced to install edison headers into the main include dir
# unless we patch the includes to add a "otbedison" root dir
# in all #include directives
install(FILES
  ${EDISON_HEADERS}
  DESTINATION ${OTB3P_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )

otb_module_target( otbedison NO_INSTALL )