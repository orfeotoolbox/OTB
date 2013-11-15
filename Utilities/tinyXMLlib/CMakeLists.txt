project(tinyXMLlib)

file(GLOB tinyXMLlib_SRCS "*.cpp")
file(GLOB tinyXMLlib_HDRS "*.h")
add_library(otbtinyXML ${tinyXMLlib_SRCS})
if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(otbtinyXML PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif()

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS otbtinyXML
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif()

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  install(FILES ${tinyXMLlib_HDRS}
          DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/tinyXMLlib
          COMPONENT Development)
endif()

set(TINYXML_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set(TINYXML_LIBRARIES otbtinyXML)
