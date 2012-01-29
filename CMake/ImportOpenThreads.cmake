MESSAGE(STATUS "Importing OpenThreads...")
#-----------------------------------------------------------------------------
# Option for OpenThreads (necessary because used by the OSSIM library)
# Find OpenThread library
IF(EXISTS "${CMAKE_ROOT}/Modules/FindOpenThreads.cmake")
        FIND_PACKAGE(OpenThreads)
ELSE(EXISTS "${CMAKE_ROOT}/Modules/FindOpenThreads.cmake")
        INCLUDE(${OTB_SOURCE_DIR}/CMake/FindOpenThreads.cmake)
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/FindOpenThreads.cmake")

MARK_AS_ADVANCED(OPENTHREADS_INCLUDE_DIR)
MARK_AS_ADVANCED(OPENTHREADS_LIBRARY)
MARK_AS_ADVANCED(OPENTHREADS_LIBRARY_DEBUG)

SET(OTB_USE_EXTERNAL_OPENTHREADS ON CACHE INTERNAL "")
IF(OPENTHREADS_FOUND)

        INCLUDE_DIRECTORIES(${OPENTHREADS_INCLUDE_DIR})
        LINK_DIRECTORIES( ${OPENTHREADS_LIBRARY} )
                
        MESSAGE(STATUS "  Using OpenThreads external version")
        MESSAGE(STATUS "  OpenThreads includes : ${OPENTHREADS_INCLUDE_DIR}")
        MESSAGE(STATUS "  OpenThreads library  : ${OPENTHREADS_LIBRARY}")

ELSE(OPENTHREADS_FOUND)

        ADD_DEFINITIONS(-DOTB_USE_INTERNAL_OPENTHREADS)
        SET(OTB_USE_EXTERNAL_OPENTHREADS OFF)
        SET(OPENTHREADS_INCLUDE_DIR "${OTB_SOURCE_DIR}/Utilities/otbopenthreads/OpenThreads/include")
        SET(OPENTHREADS_LIBRARY "OpenThreads" )
        INCLUDE_DIRECTORIES(${OPENTHREADS_INCLUDE_DIR})
        IF(NOT BUILD_SHARED_LIBS)
          ADD_DEFINITIONS(-DOT_LIBRARY_STATIC)
        ENDIF(NOT BUILD_SHARED_LIBS)
        
        MESSAGE(STATUS "  Using OpenThreads internal version")
        
ENDIF(OPENTHREADS_FOUND)
