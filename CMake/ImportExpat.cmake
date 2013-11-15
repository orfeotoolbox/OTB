message(STATUS "Importing EXPAT...")

find_package(EXPAT)

# Initialize option with what's found on the system
if(EXPAT_FOUND)
  option(OTB_USE_EXTERNAL_EXPAT "Use an outside build of EXPAT." ON)
else(EXPAT_FOUND)
  option(OTB_USE_EXTERNAL_EXPAT "Use an outside build of EXPAT." OFF)
endif(EXPAT_FOUND)
mark_as_advanced(OTB_USE_EXTERNAL_EXPAT)

# Setup
if(OTB_USE_EXTERNAL_EXPAT)
    if(EXPAT_FOUND)
      message(STATUS "Using EXPAT external version")
    else(EXPAT_FOUND)
      # Generate an error if no external EXPAT is available
      message(FATAL_ERROR "EXPAT required but not found. "
                          "Turn OTB_USE_EXTERNAL_EXPAT to OFF to use the internal version.")
    endif(EXPAT_FOUND)
else(OTB_USE_EXTERNAL_EXPAT)
    message(STATUS "Using EXPAT internal version")

    set(EXPAT_LIBRARIES otbexpat)
    add_definitions(-DOTB_USE_INTERNAL_EXPAT)
    if(NOT BUILD_SHARED_LIBS)
      add_definitions(-DXML_STATIC)
    endif(NOT BUILD_SHARED_LIBS)
    include_directories(BEFORE "${OTB_SOURCE_DIR}/Utilities/otbexpat")
    include_directories(BEFORE "${OTB_BINARY_DIR}/Utilities/otbexpat")
endif(OTB_USE_EXTERNAL_EXPAT)
