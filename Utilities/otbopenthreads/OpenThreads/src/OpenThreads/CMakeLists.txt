# This is mainly for Windows declspec, but other platforms know
# what to do with it.
add_definitions(-DOPENTHREADS_EXPORTS)

set(OPENTHREADS_MAJOR_VERSION 1)
set(OPENTHREADS_MINOR_VERSION 9)
set(OPENTHREADS_PATCH_VERSION 5)
set(OPENTHREADS_SOVERSION 7)

set(OPENTHREADS_VERSION ${OPENTHREADS_MAJOR_VERSION}.${OPENTHREADS_MINOR_VERSION}.${OPENTHREADS_PATCH_VERSION})

set(HEADER_PATH ${OpenThreads_SOURCE_DIR}/include/OpenThreads)
set(OpenThreads_PUBLIC_HEADERS
    ${HEADER_PATH}/Barrier
    ${HEADER_PATH}/Block
    ${HEADER_PATH}/Condition
    ${HEADER_PATH}/Exports
    ${HEADER_PATH}/Mutex
    ${HEADER_PATH}/ReadWriteMutex
    ${HEADER_PATH}/ReentrantMutex
    ${HEADER_PATH}/ScopedLock
    ${HEADER_PATH}/Thread
)

# User Options
# OTB modifications
#option(DYNAMIC_OPENTHREADS "Set to ON to build OpenThreads for dynamic linking.  Use OFF for static." ON)
#if(DYNAMIC_OPENTHREADS)
#    set(OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC "SHARED")
#else()
#    set(OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC "STATIC")
#endif()
if( OTB_BUILD_SHARED_LIBS )
    set(OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC "SHARED")
else()
    set(OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC "STATIC")
endif()

# Use our modified version of FindThreads.cmake which has Sproc hacks.
find_package(Threads)

# Do we have sproc?
if(CMAKE_SYSTEM MATCHES IRIX)
    if(CMAKE_USE_SPROC_INIT)
        # In this case, only Sproc exists, so no option.
        set(OPENTHREADS_USE_SPROC_INSTEAD_OF_PTHREADS 1)
    else()
        if(CMAKE_HAVE_SPROC_H)
            option(OPENTHREADS_USE_SPROC_INSTEAD_OF_PTHREADS "Set to ON to build OpenThreads against sproc instead of pthreads" OFF)
        endif()
    endif()
endif()

# Maybe we should be using the FindThreads.cmake module?
if(WIN32)
    # So I think Cygwin wants to use pthreads
    if(CYGWIN)
        subdirs(pthreads)
    else()
        # Everybody else including Msys should probably go here
        subdirs(win32)
        # examples)
    endif()
else()
    if(UNIX)
        if(OPENTHREADS_USE_SPROC_INSTEAD_OF_PTHREADS)
            subdirs(sproc)
        else()
            subdirs(pthreads)
        endif()
    else()
        message("Sorry, OpenThreads may not support your platform")
    endif()
endif()



