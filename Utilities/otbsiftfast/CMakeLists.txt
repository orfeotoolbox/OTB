cmake_minimum_required (VERSION 2.4.7)
project (otbsiftfast)
set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )

include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckCXXSourceRuns)
include(CheckCXXCompilerFlag)

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
