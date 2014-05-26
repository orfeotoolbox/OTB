message(STATUS "Importing MuParser...")

option(OTB_USE_EXTERNAL_MUPARSER "Use external MuParser library." ON)
mark_as_advanced(OTB_USE_EXTERNAL_MUPARSER)

if(OTB_USE_EXTERNAL_MUPARSER)
  find_package(MuParser)

  if(MUPARSER_FOUND)
    # Starting with muparser 2.0.0,
    #  intrinsic operators "and", "or", "xor" have been removed
    #  and intrinsic operators "&&" and "||" have been introduced as replacements
    set(CMAKE_REQUIRED_INCLUDES ${MUPARSER_INCLUDE_DIR})
    set(CMAKE_REQUIRED_LIBRARIES ${MUPARSER_LIBRARIES})
    unset(CMAKE_REQUIRED_FLAGS)
    unset(CMAKE_REQUIRED_DEFINES)
    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otbTestMuParserHasCxxLogicalOperators.cxx
        OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS_SOURCEFILE)
    check_cxx_source_runs(
      "${OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS_SOURCEFILE}"
      OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
      )
    unset(CMAKE_REQUIRED_INCLUDES)
    unset(CMAKE_REQUIRED_LIBRARIES)
    unset(CMAKE_REQUIRED_FLAGS)
    unset(CMAKE_REQUIRED_DEFINES)

    if(OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS)
      message(STATUS "  Testing if muParser has '&&' operator -- yes")
    else()
      message(STATUS "  Testing if muParser has '&&' operator -- no")
    endif()
  else()
    message(FATAL_ERROR "Can't build OTB without MuParser. Instal MuParser on your system, or disable the option OTB_USE_EXTERNAL_MUPARSER")
  endif()

else()

  set(MUPARSER_LIBRARIES otbmuparser)
  unset(OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS) # internal version is < 2.0.0
  message(STATUS "  Using MuParser internal version (no '&&' operator)")
endif()

