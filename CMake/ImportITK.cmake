MESSAGE(STATUS "Importing ITK...")

OPTION(OTB_USE_EXTERNAL_ITK "Use an outside build of ITK." OFF)
MARK_AS_ADVANCED(OTB_USE_EXTERNAL_ITK)

IF(OTB_USE_EXTERNAL_ITK)
  FIND_PACKAGE(ITK)
  IF(ITK_FOUND)
        # Build ITK_INCLUDE_DIRS list wihout expat path
        SET(new_list "")
        FOREACH(files_h ${ITK_INCLUDE_DIRS})
                IF(NOT "${files_h}" MATCHES "expat")
                        SET(new_list "${new_list};${files_h}")
                ENDIF(NOT "${files_h}" MATCHES "expat")
        ENDFOREACH(files_h ${ITK_INCLUDE_DIRS})
        SET(ITK_INCLUDE_DIRS "${new_list}")

        INCLUDE(${ITK_USE_FILE})
  ELSE(ITK_FOUND)
        MESSAGE(FATAL_ERROR
                  "Cannot build OTB project without ITK.  Please set ITK_DIR or set OTB_USE_EXTERNAL_ITK OFF to use INTERNAL ITK set on OTB/Utilities repository.")
  ENDIF(ITK_FOUND)

  # Build shared libraries for otb
  # By default, if otb use an external itk import the option from the ITK configuration
  SET(BUILD_SHARED_LIBS ${ITK_BUILD_SHARED})

  MESSAGE(STATUS "  Using ITK external version")

ELSE(OTB_USE_EXTERNAL_ITK)


  # Required to have proper itk headers at first run
  SET(ITK_SOURCE_DIR ${OTB_SOURCE_DIR}/Utilities/ITK)
  SET(ITK_BINARY_DIR ${OTB_BINARY_DIR}/Utilities/ITK)
  MESSAGE(STATUS "  Using ITK internal version")

ENDIF(OTB_USE_EXTERNAL_ITK)
