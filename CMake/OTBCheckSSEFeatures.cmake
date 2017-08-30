function(check_sse_features sse_flags )
  include(CheckCXXSourceRuns)
  include(CheckCXXCompilerFlag)

  set(${sse_flags} "0" PARENT_SCOPE)
  # For apple assume sse2 is on for all intel builds, check for 64 and 32 bit versions
  if(APPLE)
      message(STATUS "For apple assume sse2 is on for all intel builds")
    return()
  endif()

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
     }" HAVE_SSE2_FLAGS)
 
    if(HAVE_SSE2_FLAGS)
      set(${sse_flags} "-msse2 -mfpmath=sse" PARENT_SCOPE)
      return()
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
    }" HAVE_SSE_FLAGS)
    
    if(HAVE_SSE_FLAGS)
      set(${sse_flags} "-msse -mfpmath=sse" PARENT_SCOPE)
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
        }" HAVE_SSE_FLAGS)
    
    if(HAVE_SSE_FLAGS)
      message(STATUS "Performing Test HAVE_SSE_FLAGS - Success")
      set(${sse_flags} "/arch:SSE2 /fp:fast -D__SSE__ -D__SSE2__" PARENT_SCOPE)
      return()
    else()
      message(STATUS "Performing Test HAVE_SSE_FLAGS - Failed")
      return()
    endif()
endif()
endfunction() #check_sse_features
