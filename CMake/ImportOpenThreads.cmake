message(STATUS "Importing OpenThreads...")

find_package(OpenThreads)

mark_as_advanced(OPENTHREADS_INCLUDE_DIR)
mark_as_advanced(OPENTHREADS_LIBRARY)
mark_as_advanced(OPENTHREADS_LIBRARY_DEBUG)

set(OTB_USE_EXTERNAL_OPENTHREADS ON CACHE INTERNAL "")
if(OPENTHREADS_FOUND)

        include_directories(${OPENTHREADS_INCLUDE_DIR})
        link_directories( ${OPENTHREADS_LIBRARY} )

        message(STATUS "  Using OpenThreads external version")
        message(STATUS "  OpenThreads includes : ${OPENTHREADS_INCLUDE_DIR}")
        message(STATUS "  OpenThreads library  : ${OPENTHREADS_LIBRARY}")

else()

        add_definitions(-DOTB_USE_INTERNAL_OPENTHREADS)
        set(OTB_USE_EXTERNAL_OPENTHREADS OFF)
        set(OPENTHREADS_INCLUDE_DIR "${OTB_SOURCE_DIR}/Utilities/otbopenthreads/OpenThreads/include")
        set(OPENTHREADS_LIBRARY "OpenThreads" )
        include_directories(${OPENTHREADS_INCLUDE_DIR})
        if(NOT BUILD_SHARED_LIBS)
          add_definitions(-DOT_LIBRARY_STATIC)
        endif()

        message(STATUS "  Using OpenThreads internal version")

endif()
