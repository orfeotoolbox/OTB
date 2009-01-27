cmake_minimum_required (VERSION 2.4.7)
project (otbsiftfast)
set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )

include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckCXXSourceRuns)
include(CheckCXXCompilerFlag)
include(CheckCXXSourceCompiles)

# check for SSE extensions
if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
  set(SSE_FLAGS)

  set(CMAKE_REQUIRED_FLAGS "-msse2")
  check_cxx_source_runs("
    #include <emmintrin.h>

    int main()
    {
        __m128d a, b;
        double vals[2] = {0};
        a = _mm_loadu_pd(vals);
        b = _mm_add_pd(a,a);
        _mm_storeu_pd(vals,b);
        return 0;
     }"
     HAS_SSE2_EXTENSIONS)

  set(CMAKE_REQUIRED_FLAGS "-msse")
  check_cxx_source_runs("
    #include <xmmintrin.h>
    int main()
    {
        __m128 a, b;
        float vals[4] = {0};
        a = _mm_loadu_ps(vals);
        b = a;
        b = _mm_add_ps(a,b);
        _mm_storeu_ps(vals,b);
        return 0;
    }"
    HAS_SSE_EXTENSIONS)

  set(CMAKE_REQUIRED_FLAGS)

  if(HAS_SSE2_EXTENSIONS)
    message(STATUS "Using SSE2 extensions")
    set(SSE_FLAGS "-msse2 -mfpmath=sse")
  elseif(HAS_SSE_EXTENSIONS)
    message(STATUS "Using SSE extensions")
    set(SSE_FLAGS "-msse -mfpmath=sse")
  endif()

  add_definitions(${SSE_FLAGS})
elseif(MSVC)
  check_cxx_source_runs("
    #include <emmintrin.h>

    int main()
    {
        __m128d a, b;
        double vals[2] = {0};
        a = _mm_loadu_pd(vals);
        b = _mm_add_pd(a,a);
        _mm_storeu_pd(vals,b);
        return 0;
     }"
     HAS_SSE2_EXTENSIONS)
  if( HAS_SSE2_EXTENSIONS )
    message(STATUS "Using SSE2 extensions")
    add_definitions( "/arch:SSE2 /fp:fast -D__SSE__ -D__SSE2__" )
  endif()
endif()

# TRY  COMPILE libsiftfast.cpp (depend of gcc version)
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(IS_SIFTFAST_COMPILE 0)
    TRY_COMPILE(IS_SIFTFAST_COMPILE
              ${CMAKE_CURRENT_BINARY_DIR}
              ${CMAKE_CURRENT_SOURCE_DIR}/test_try_compile_libsiftfast.cpp
              CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${SSE_FLAGS}
              OUTPUT_VARIABLE OUTPUT)
    IF(IS_SIFTFAST_COMPILE)
        MESSAGE(STATUS "Try to compile libsiftfast.cpp -- yes.")
    ELSE(IS_SIFTFAST_COMPILE)
        MESSAGE(STATUS "Try to compile libsiftfast.cpp -- no.")
        TRY_COMPILE(IS_SIFTFAST_COMPILE2
              ${CMAKE_CURRENT_BINARY_DIR}
              ${CMAKE_CURRENT_SOURCE_DIR}/test_try_compile_libsiftfast.cpp
              CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${SSE_FLAGS}
              COMPILE_DEFINITIONS -DOTB_DISABLE_FAST_FUNCTIONS
              OUTPUT_VARIABLE OUTPUT)
        IF(IS_SIFTFAST_COMPILE2)
            MESSAGE(STATUS "Try to compile libsiftfast.cpp with disabling fast functions (add CFLAGS -DOTB_DISABLE_FAST_FUNCTIONS) -- yes.")
        ELSE(IS_SIFTFAST_COMPILE2)
            MESSAGE(STATUS "Try to compile libsiftfast.cpp with disabling fast functions (add CFLAGS -DOTB_DISABLE_FAST_FUNCTIONS) -- no.")
            MESSAGE(FATAL_ERROR "Impossible to compile libsiftfast. Contact OTB development team")
        ENDIF(IS_SIFTFAST_COMPILE2)
        SET(OTB_DISABLE_FAST_FUNCTIONS_VALUE 1)
    ELSE(NOT IS_SIFTFAST_COMPILE)
        MESSAGE(STATUS "Try to compile libsiftfast.cpp done.")
    ENDIF(IS_SIFTFAST_COMPILE)

    IF(OTB_DISABLE_FAST_FUNCTIONS_VALUE)
        ADD_DEFINITIONS(-DOTB_DISABLE_FAST_FUNCTIONS)
    ENDIF(OTB_DISABLE_FAST_FUNCTIONS_VALUE)

ENDIF(CMAKE_COMPILER_IS_GNUCXX)


IF(CMAKE_COMPILER_IS_GNUCXX)
      SET_SOURCE_FILES_PROPERTIES(libsiftfast.cpp PROPERTIES COMPILE_FLAGS -w)
ELSE(CMAKE_COMPILER_IS_GNUCXX)
  IF(WIN32)
    IF (CMAKE_CXX_COMPILER MATCHES "^cl$")
        SET_SOURCE_FILES_PROPERTIES( libsiftfast.cpp PROPERTIES COMPILE_FLAGS "/W0" )
    ENDIF (CMAKE_CXX_COMPILER MATCHES "^cl$")
  ENDIF(WIN32)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
#-fstrict-aliasing

add_library(otbsiftfast libsiftfast.cpp)

# check for OpenMP
if( NOT DEFINED USE_OPENMP OR USE_OPENMP  )

  if( WIN32 )
    CHECK_INCLUDE_FILE(omp.h HAVE_OMP_H)
    if( HAVE_OMP_H )
      message(STATUS "Using OpenMP")
      check_cxx_compiler_flag(/openmp HAVE_OPENMP)

      if( HAVE_OPENMP )
        add_definitions("/openmp")
      endif()
    endif()
  elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)

    # check if compilers supports -fopenmp
    INCLUDE(CheckCCompilerFlag)
    check_cxx_compiler_flag(-fopenmp HAVE_OPENMP)
    check_library_exists(gomp omp_get_num_threads "" HAS_GOMP_LIB)

    if( HAVE_OPENMP AND HAS_GOMP_LIB )
      add_definitions("-fopenmp")
      target_link_libraries(otbsiftfast gomp)
      set(OPENMP_LFLAGS "-lgomp")
    endif()
  endif()
endif()

#Instal TARGET & FILES for otb-lib

INSTALL(TARGETS otbsiftfast
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

INSTALL(FILES siftfast.h 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbsiftfast
    COMPONENT Development)
