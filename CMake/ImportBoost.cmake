MESSAGE(STATUS "Importing Boost...")
SET (Boost_MINOR_VERSION 0)

OPTION(OTB_USE_EXTERNAL_BOOST "Use an outside build of Boost." OFF)
MARK_AS_ADVANCED(OTB_USE_EXTERNAL_BOOST)

IF(OTB_USE_EXTERNAL_BOOST)
  # Import the package
  FIND_PACKAGE(Boost)
  MARK_AS_ADVANCED(Boost_DIR)
  IF(Boost_FOUND)
    MESSAGE(STATUS "  Found Boost version ${Boost_VERSION}")
    IF( ${Boost_MINOR_VERSION} LESS 35)
      MESSAGE(WARNING "Boost >= 1.35 is required. Disabling OTB_USE_EXTERNAL_BOOST")
      SET(OTB_USE_EXTERNAL_BOOST  OFF)
    ENDIF(${Boost_MINOR_VERSION} LESS 35)
  ELSE(Boost_FOUND)
    MESSAGE(FATAL_ERROR
                  "Cannot build OTB project without boost library.  Please set Boost directories or set OTB_USE_EXTERNAL_BOOST to OFF to use the INTERNAL Boost version")
  ENDIF(Boost_FOUND)

  # Test package completeness
  TRY_COMPILE(BOOST_IS_COMPLETE
              ${CMAKE_CURRENT_BINARY_DIR}/CMake
              ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otbTestCompileBoost.cxx
              CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${Boost_INCLUDE_DIR}"
                          "-DLINK_DIRECTORIES:PATH=${Boost_LIBRARY_DIRS}"
              COMPILE_DEFINITIONS "-DBOOST_SERIALIZATION_DYN_LINK"
              OUTPUT_VARIABLE OUTPUT)

  IF(BOOST_IS_COMPLETE)
    MESSAGE(STATUS "  Testing external Boost library    -- yes")

    # needed for automatic linking on msvc platform
    LINK_DIRECTORIES( ${Boost_LIBRARY_DIRS} )

  ELSE(BOOST_IS_COMPLETE)
    MESSAGE(STATUS "  Testing external Boost library    -- no")
    MESSAGE(STATUS "  Forcing the OTB_USE_EXTERNAL_BOOST value to OFF")
    SET(OTB_USE_EXTERNAL_BOOST OFF CACHE BOOL  "Use an outside build of Boost" FORCE )
  ENDIF(BOOST_IS_COMPLETE)
ENDIF(OTB_USE_EXTERNAL_BOOST)


IF(OTB_USE_EXTERNAL_BOOST)
  MESSAGE(STATUS "  Using Boost external version")
  MESSAGE(STATUS "  Boost includes : ${Boost_INCLUDE_DIR}")
ELSE(OTB_USE_EXTERNAL_BOOST)
  MESSAGE(STATUS "  Using Boost internal version")
ENDIF(OTB_USE_EXTERNAL_BOOST)
