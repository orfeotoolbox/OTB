message(STATUS "Importing MuParserX...")
find_package(MuParserX)

if(MUPARSERX_FOUND)
  option(OTB_USE_EXTERNAL_MUPARSERX "Use external MuParserX library." ON)
else()
  option(OTB_USE_EXTERNAL_MUPARSERX "Use external MuParserX library." OFF)
endif()
mark_as_advanced(OTB_USE_EXTERNAL_MUPARSERX)

if(OTB_USE_EXTERNAL_MUPARSERX)
  if(MUPARSERX_FOUND)
    # Starting with muparser 2.0.0,
    #  intrinsic operators "and", "or", "xor" have been removed
    #  and intrinsic operators "&&" and "||" have been introduced as replacements
    set(CMAKE_REQUIRED_INCLUDES ${MUPARSERX_INCLUDE_DIR})
    set(CMAKE_REQUIRED_LIBRARIES ${MUPARSERX_LIBRARIES})
    unset(CMAKE_REQUIRED_FLAGS)
    unset(CMAKE_REQUIRED_DEFINES)
    #file(READ ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otbTestMuParserHasCxxLogicalOperators.cxx
    #    OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS_SOURCEFILE)
    #check_cxx_source_runs(
    #  "${OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS_SOURCEFILE}"
    #  OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
    #  )
    unset(CMAKE_REQUIRED_INCLUDES)
    unset(CMAKE_REQUIRED_LIBRARIES)
    unset(CMAKE_REQUIRED_FLAGS)
    unset(CMAKE_REQUIRED_DEFINES)

    #if(OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS)
    #  message(STATUS "  MuParser version is >= 2.0.0 : "
    #                 "uses '&&' and '||' logical operators, and C++ like "
    #                 "ternary if-then-else operator")
    #else()
    #  message(STATUS "  MuParser version is < 2.0.0  : "
    #                 "uses 'and' and 'or' logical operators, and ternary "
    #                 "operator 'if( ; ; )'")
    #endif()
  else()
    message(FATAL_ERROR "Can't build OTB without MuParserX. Instal it "
                        "on your system, or disable the option "
                        "OTB_USE_EXTERNAL_MUPARSERX to use internal one")
  endif()
else()
  set(MUPARSERX_LIBRARIES otbmuparserx)
  #unset(OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS) # internal version is < 2.0.0
  message(STATUS "  Using MuParserX internal version ")
endif()

