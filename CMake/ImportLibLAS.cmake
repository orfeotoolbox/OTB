message(STATUS "Importing LibLAS...")
option(OTB_USE_LIBLAS "Use liblas library to support Lidar data format." ON)
mark_as_advanced(OTB_USE_LIBLAS)

if(OTB_USE_LIBLAS)
  find_package(LibLAS)

  # Initialize option with what's found on the system
  if(LIBLAS_FOUND)
    option(OTB_USE_EXTERNAL_LIBLAS "Use external LIBLAS library." ON)
  else(LIBLAS_FOUND)
    option(OTB_USE_EXTERNAL_LIBLAS "Use external LIBLAS library." OFF)
  endif(LIBLAS_FOUND)
  mark_as_advanced(OTB_USE_EXTERNAL_LIBLAS)

  if(OTB_USE_EXTERNAL_LIBLAS)
    if(LIBLAS_FOUND)
      message(STATUS "Using LibLAS external version")
    else(LIBLAS_FOUND)
      # Generate an error if no external liblas is available
      message(FATAL_ERROR "LibLAS required but not found. "
                          "Either turn OTB_USE_EXTERNAL_LIBLAS to OFF to use the internal version, "
                          "or OTB_USE_LIBLAS to OFF to disable LAS support")
    endif(LIBLAS_FOUND)
  else(OTB_USE_EXTERNAL_LIBLAS)
    message(STATUS "Using LibLAS internal version")
    set(LIBLAS_LIBRARIES otbliblas)
    add_definitions(-DLAS_DISABLE_DLL)
  endif(OTB_USE_EXTERNAL_LIBLAS)

endif(OTB_USE_LIBLAS)
