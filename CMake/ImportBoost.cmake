message(STATUS "Importing Boost...")
set(Boost_MINOR_VERSION 0)

option(OTB_USE_EXTERNAL_BOOST "Use an outside build of Boost." OFF)
mark_as_advanced(OTB_USE_EXTERNAL_BOOST)

if(OTB_USE_EXTERNAL_BOOST)
  # Import the package
  find_package(Boost 1.35.0)
  mark_as_advanced(Boost_DIR)
  if(Boost_FOUND)
    message(STATUS "  Found Boost version ${Boost_VERSION}")
  else()
    message(FATAL_ERROR
                  "Cannot build OTB project without boost library (>1.35.0). Please set Boost directories or set OTB_USE_EXTERNAL_BOOST to OFF to use the INTERNAL Boost version")
  endif()

  # Test package completeness
  try_compile(BOOST_IS_COMPLETE
              ${CMAKE_CURRENT_BINARY_DIR}/CMake
              ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otbTestCompileBoost.cxx
              CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${Boost_INCLUDE_DIR}"
                          "-DLINK_DIRECTORIES:PATH=${Boost_LIBRARY_DIRS}"
              COMPILE_DEFINITIONS "-DBOOST_SERIALIZATION_DYN_LINK"
              OUTPUT_VARIABLE OUTPUT)

  if(BOOST_IS_COMPLETE)
    message(STATUS "  Testing external Boost library    -- yes")

    # needed for automatic linking on msvc platform
    link_directories( ${Boost_LIBRARY_DIRS} )

  else()
    message(STATUS "  Testing external Boost library    -- no")
    message(STATUS "  Forcing the OTB_USE_EXTERNAL_BOOST value to OFF")
    set(OTB_USE_EXTERNAL_BOOST OFF CACHE BOOL  "Use an outside build of Boost" FORCE )
  endif()
endif()


if(OTB_USE_EXTERNAL_BOOST)
  message(STATUS "  Using Boost external version")
  message(STATUS "  Boost includes : ${Boost_INCLUDE_DIR}")
else()
  message(STATUS "  Using Boost internal version")
endif()
