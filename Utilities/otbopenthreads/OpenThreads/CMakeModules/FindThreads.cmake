# - This module determines the thread library of the system.
# The following variables are set
#  CMAKE_THREAD_LIBS_INIT     - the thread library
#  CMAKE_USE_SPROC_INIT       - are we using sproc?
#  CMAKE_USE_WIN32_THREADS_INIT - using WIN32 threads?
#  CMAKE_USE_PTHREADS_INIT    - are we using pthreads
#  CMAKE_HP_PTHREADS_INIT     - are we using hp pthreads

# This module is a modification of the version found in the official
# CMake distribution. The problem is that the built in version's Sproc
# handling is undesirable for us. The official module assumes you use
# Sproc if it exists on your system. We assume you might have both Pthreads
# and Sproc on your system in which case the user needs to choose.
# Furthermore, we prefer to provide Pthreads as the default option instead
# of Sproc.

include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)

# Do we have sproc?
if(CMAKE_SYSTEM MATCHES IRIX)
  CHECK_INCLUDE_FILES("sys/types.h;sys/prctl.h"  CMAKE_HAVE_SPROC_H)
endif()

CHECK_INCLUDE_FILE("pthread.h" CMAKE_HAVE_PTHREAD_H)
if(CMAKE_HAVE_PTHREAD_H)
    # We have pthread.h
    # Let's check for the library now.
    set(CMAKE_HAVE_THREADS_LIBRARY)
    if(NOT THREADS_HAVE_PTHREAD_ARG)
      # Do we have -lpthreads
      CHECK_LIBRARY_EXISTS(pthreads pthread_create "" CMAKE_HAVE_PTHREADS_CREATE)
      if(CMAKE_HAVE_PTHREADS_CREATE)
        set(CMAKE_THREAD_LIBS_INIT "-lpthreads")
        set(CMAKE_HAVE_THREADS_LIBRARY 1)
      endif()
      # Ok, how about -lpthread
      CHECK_LIBRARY_EXISTS(pthread pthread_create "" CMAKE_HAVE_PTHREAD_CREATE)
      if(CMAKE_HAVE_PTHREAD_CREATE)
        set(CMAKE_THREAD_LIBS_INIT "-lpthread")
        set(CMAKE_HAVE_THREADS_LIBRARY 1)
      endif()
      if(CMAKE_SYSTEM MATCHES "SunOS.*")
        # On sun also check for -lthread
        CHECK_LIBRARY_EXISTS(thread thr_create "" CMAKE_HAVE_THR_CREATE)
        if(CMAKE_HAVE_THR_CREATE)
          set(CMAKE_THREAD_LIBS_INIT "-lthread")
          set(CMAKE_HAVE_THREADS_LIBRARY 1)
        endif()
      endif()
    endif()

    if(NOT CMAKE_HAVE_THREADS_LIBRARY)
      # If we did not found -lpthread, -lpthread, or -lthread, look for -pthread
      if("THREADS_HAVE_PTHREAD_ARG" MATCHES "^THREADS_HAVE_PTHREAD_ARG")
        message(STATUS "Check if compiler accepts -pthread")
        try_run(THREADS_PTHREAD_ARG THREADS_HAVE_PTHREAD_ARG
          ${CMAKE_BINARY_DIR}
          ${CMAKE_ROOT}/Modules/CheckForPthreads.c
          CMAKE_FLAGS -DLINK_LIBRARIES:STRING=-pthread
          OUTPUT_VARIABLE OUTPUT)
        if(THREADS_HAVE_PTHREAD_ARG)
          if(THREADS_PTHREAD_ARG MATCHES "^2$")
            message(STATUS "Check if compiler accepts -pthread - yes")
          else()
            message(STATUS "Check if compiler accepts -pthread - no")
            file(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
              "Determining if compiler accepts -pthread returned ${THREADS_PTHREAD_ARG} instead of 2. The compiler had the following output:\n${OUTPUT}\n\n")
          endif()
        else()
          message(STATUS "Check if compiler accepts -pthread - no")
          file(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
            "Determining if compiler accepts -pthread failed with the following output:\n${OUTPUT}\n\n")
        endif()
      endif()
      if(THREADS_HAVE_PTHREAD_ARG)
        set(CMAKE_THREAD_LIBS_INIT "-pthread")
      else()
      endif()
    endif()
else()
	if(CMAKE_HAVE_SPROC_H)
	  # We have sproc but not pthread so the answer is obvious.
	  set(CMAKE_USE_SPROC_INIT 1)
	endif()
endif()

if(CMAKE_THREAD_LIBS_INIT)
  set(CMAKE_USE_PTHREADS_INIT 1)
endif()

if(CMAKE_SYSTEM MATCHES "Windows")
  set(CMAKE_USE_WIN32_THREADS_INIT 1)
endif()

if(CMAKE_USE_PTHREADS_INIT)
  if(CMAKE_SYSTEM MATCHES "HP-UX-*")
    set(CMAKE_THREAD_LIBS_INIT "-lcma")
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(CMAKE_HP_PTHREADS_INIT 1)
  endif()

  if(CMAKE_SYSTEM MATCHES "OSF1-V*")
    set(CMAKE_USE_PTHREADS_INIT 0)
    set(CMAKE_THREAD_LIBS_INIT )
  endif()

  if(CMAKE_SYSTEM MATCHES "CYGWIN_NT*")
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(CMAKE_THREAD_LIBS_INIT )
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
  endif()
endif()

