message(STATUS "Importing TinyXML...")
find_package(TinyXML)

if(TINYXML_FOUND)
  option(OTB_USE_EXTERNAL_TINYXML "Use external TinyXML library." ON)
else()
  option(OTB_USE_EXTERNAL_TINYXML "Use external TinyXML library." OFF)
endif()
mark_as_advanced(OTB_USE_EXTERNAL_TINYXML)

if(OTB_USE_EXTERNAL_TINYXML)
  if(TINYXML_FOUND)
    # Check if STL support is enabled with macro TIXML_USE_STL
    try_compile(TIXML_USE_STL
                ${CMAKE_CURRENT_BINARY_DIR}/CMake
                ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otbTestTinyXMLUseSTL.cxx
                CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${TINYXML_INCLUDE_DIRS}" "-DLINK_LIBRARIES:STRING=${TINYXML_LIBRARIES}"
                COMPILE_DEFINITIONS "-DTIXML_USE_STL"
                OUTPUT_VARIABLE OUTPUT)
  else()
    message(FATAL_ERROR "Can't build OTB without TinyXML. Install it "
                        "on your system, or disable the option "
                        "OTB_USE_EXTERNAL_TINYXML to use the internal one")
  endif()
else()
  set(TINYXML_LIBRARIES otbtinyXML)
  set(TIXML_USE_STL)
  message(STATUS "  Using TinyXML internal version")
endif()
