message(STATUS "Importing Ossim...")

set(DEFAULT_OTB_USE_EXTERNAL_OSSIM  OFF)

# OSGeo4W provides an "ossim" package : use it otherwise if it is installed and not used by OTB
# we get conflicts (because OSGeo4W include dir is included for other dependencies
if(WIN32)
  set(DEFAULT_OTB_USE_EXTERNAL_OSSIM  ON)
endif()

option(OTB_USE_EXTERNAL_OSSIM "  Use an outside build of Ossim." ${DEFAULT_OTB_USE_EXTERNAL_OSSIM})
mark_as_advanced(OTB_USE_EXTERNAL_OSSIM)

if(OTB_USE_EXTERNAL_OSSIM)
  find_package(Ossim REQUIRED)
  if(OSSIM_FOUND)
    set(OSSIM_INCLUDE_DIRS ${OSSIM_INCLUDE_DIR})
    set(OSSIM_LIBRARIES ${OSSIM_LIBRARY})
    message(STATUS "  Found OSSIM library at ${OSSIM_LIBRARY}")
    message(STATUS "  OSSIM version is " ${OSSIM_VERSION})
    message(STATUS "  Using OSSIM external version")
  else()
    set(OSSIM_INCLUDE_DIRS)
    set(OSSIM_LIBRARIES)
    #No external ossim found
    message(FATAL_ERROR "OSSIM library is required but not found. "
      "Turn OTB_USE_EXTERNAL_OSSIM to OFF to use the internal version.")
  endif()
else()
  set(OSSIM_LIBRARIES otbossim)
  if(WIN32 AND NOT BUILD_SHARED_LIBS)
    add_definitions(-DOSSIM_STATIC)
  endif()
  message(STATUS "  Using OSSIM internal version")
endif()
