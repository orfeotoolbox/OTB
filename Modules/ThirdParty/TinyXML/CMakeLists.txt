project(OTBTinyXML)

# Check if we need to define TIXML_USE_STL
try_compile( TIXML_USE_STL
             ${CMAKE_CURRENT_BINARY_DIR}/CMake
             ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otbTestTinyXMLUseSTL.cxx
             CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${TINYXML_INCLUDE_DIRS}" "-DLINK_LIBRARIES:STRING=${TINYXML_LIBRARIES}"
             COMPILE_DEFINITIONS "-DTIXML_USE_STL"
             OUTPUT_VARIABLE OUTPUT)

configure_file( src/otb_tinyxml.h.in src/otb_tinyxml.h )

set(OTBTinyXML_SYSTEM_INCLUDE_DIRS
  ${TINYXML_INCLUDE_DIRS}
  )
set(OTBTinyXML_INCLUDE_DIRS
  ${OTBTinyXML_BINARY_DIR}/src
  )
set(OTBTinyXML_LIBRARIES
  "${TINYXML_LIBRARIES}"
  )

otb_module_impl()

install(FILES ${OTBTinyXML_BINARY_DIR}/src/otb_tinyxml.h
  DESTINATION ${OTBTinyXML_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
