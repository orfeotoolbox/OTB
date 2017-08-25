function(check_sse_features sse_flags )
  include(CheckIncludeFile)
  include(CheckLibraryExists)
  include(CheckCXXSourceRuns)
  include(CheckCXXCompilerFlag)

  set(has_sse FALSE)
  set(${sse_flags} "0" PARENT_SCOPE)
  if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
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
      has_sse)
 
    if(has_sse)
      message(STATUS "Performing Test CXX_HAS_SSE2 - Success")
      set(${sse_flags} "-msse2 -mfpmath=sse" PARENT_SCOPE)
    else()
      message(STATUS "Performing Test CXX_HAS_SSE2 - Failed")
    endif()

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
      has_sse)
    
    if(has_sse)
      message(STATUS "Performing Test CXX_HAS_SSE - Success")
      set(${sse_flags} "-msse -mfpmath=sse" PARENT_SCOPE)
      return()
    else()
      message(STATUS "Performing Test CXX_HAS_SSE - Failed")
      return()
    endif()
    
  elseif(MSVC)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      return()
    endif()
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
      has_sse)
    
    if(has_sse)
      message(STATUS "Performing Test CXX_HAS_SSE - Success")
      set(${sse_flags} "/arch:SSE2 /fp:fast -D__SSE__ -D__SSE2__" PARENT_SCOPE)
      return()
    else()
      message(STATUS "Performing Test CXX_HAS_SSE - Failed")
      return()
    endif()
endif()
endfunction() #check_sse_features
