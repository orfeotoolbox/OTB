MESSAGE(STATUS "Importing ITK...")

OPTION(OTB_USE_EXTERNAL_ITK "Use an outside build of ITK." ON)
MARK_AS_ADVANCED(OTB_USE_EXTERNAL_ITK)

IF(OTB_USE_EXTERNAL_ITK)
  FIND_PACKAGE(ITK REQUIRED)
  IF(ITK_FOUND)
    # This section corresponds to the content of ${ITK_USE_FILE}
    # with the exception that include_directories() is removed
    # and done in otbIncludeDirectories

    # Add compiler flags needed to use ITK.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ITK_REQUIRED_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ITK_REQUIRED_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ITK_REQUIRED_LINK_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${ITK_REQUIRED_LINK_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${ITK_REQUIRED_LINK_FLAGS}")

    # Add link directories needed to use ITK.
    link_directories(${ITK_LIBRARY_DIRS})
  ELSE(ITK_FOUND)
        MESSAGE(FATAL_ERROR
                  "Cannot build OTB project without ITK.  Please set ITK_DIR or set OTB_USE_EXTERNAL_ITK OFF to use INTERNAL ITK set on OTB/Utilities repository.")
  ENDIF(ITK_FOUND)

  # Build shared libraries for otb
  # By default, if otb use an external itk import the option from the ITK configuration
  SET(BUILD_SHARED_LIBS ${ITK_BUILD_SHARED})
  MESSAGE(STATUS "  Using ITK external version")

ELSE(OTB_USE_EXTERNAL_ITK)

  MESSAGE(STATUS "  Using ITK internal version")

ENDIF(OTB_USE_EXTERNAL_ITK)
