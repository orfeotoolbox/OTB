set(OTB3P_INSTALL_EXPORT_NAME "${OTBSiftFast-targets}")
set(OTB3P_INSTALL_INCLUDE_DIR "${OTBSiftFast_INSTALL_INCLUDE_DIR}")
set(OTB3P_INSTALL_RUNTIME_DIR "${OTBSiftFast_INSTALL_RUNTIME_DIR}")
set(OTB3P_INSTALL_LIBRARY_DIR "${OTBSiftFast_INSTALL_LIBRARY_DIR}")
set(OTB3P_INSTALL_ARCHIVE_DIR "${OTBSiftFast_INSTALL_ARCHIVE_DIR}")

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

include(CheckCXXSourceRuns)

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
  # TRY  COMPILE libsiftfast.cpp (depend of gcc version)
    set(IS_SIFTFAST_COMPILE 0)
    try_compile(IS_SIFTFAST_COMPILE
              ${CMAKE_CURRENT_BINARY_DIR}
              ${CMAKE_CURRENT_SOURCE_DIR}/test_try_compile_libsiftfast.cpp
              OUTPUT_VARIABLE OUTPUT)
    if(IS_SIFTFAST_COMPILE)
        message(STATUS "Try to compile libsiftfast.cpp -- yes.")
    else()
        message(STATUS "Try to compile libsiftfast.cpp -- no.")
        try_compile(IS_SIFTFAST_COMPILE2
              ${CMAKE_CURRENT_BINARY_DIR}
              ${CMAKE_CURRENT_SOURCE_DIR}/test_try_compile_libsiftfast.cpp
              COMPILE_DEFINITIONS -DOTB_DISABLE_FAST_FUNCTIONS
              OUTPUT_VARIABLE OUTPUT)
        if(IS_SIFTFAST_COMPILE2)
            message(STATUS "Try to compile libsiftfast.cpp with disabling fast functions (add CFLAGS -DOTB_DISABLE_FAST_FUNCTIONS) -- yes.")
        else()
            message(STATUS "Try to compile libsiftfast.cpp with disabling fast functions (add CFLAGS -DOTB_DISABLE_FAST_FUNCTIONS) -- no.")
            message(FATAL_ERROR "Impossible to compile libsiftfast. Contact OTB development team")
        endif()
        set(OTB_DISABLE_FAST_FUNCTIONS_VALUE 1)
    else()
        message(STATUS "Try to compile libsiftfast.cpp done.")
    endif()

  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g")
  add_definitions(" -Wall -fPIC ")

  # check for x86-64 system
  check_cxx_source_runs("
  int main()
  {
    int a = 0;
    int*pa = &a;
    asm(\".intel_syntax\\\\n\"
	\"mov %%rax, %0\\\\n\"
    \"mov %%eax, [%%rax]\\\\n\"
    \".att_syntax\\\\n\"
    : : \"r\"(pa) : \"%rax\");
    return 0;
  }"
  IS_X86_64)

  if( IS_X86_64 )
    add_definitions("-D__x86_64__")
  endif()
else()
  set(IS_X86_64 0)
endif()

if(OTB_DISABLE_FAST_FUNCTIONS_VALUE)
    add_definitions(-DOTB_DISABLE_FAST_FUNCTIONS)
endif()


include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckCXXSourceRuns)
include(CheckCXXCompilerFlag)

# if( UNIX OR CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
#   set(STDC_LIBRARY stdc++)
# else()
#   set(STDC_LIBRARY)
# endif()

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
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      message(STATUS "Not using SSE2 extensions on x64")
    else()
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
endif()

#set(Boost_ADDITIONAL_VERSIONS "1.40" "1.39" "1.38" "1.37.0" "1.37" "1.35.0" "1.34.1" "1.34.0" "1.34" "1.33.1" "1.33.0" "1.33")
#if( NOT $ENV{BOOST_INCLUDEDIR} STREQUAL "" )
#  set(Boost_INCLUDE_DIR $ENV{BOOST_INCLUDEDIR})
#endif()
#if( NOT $ENV{BOOST_LIBRARYDIR} STREQUAL "" )
#  set(Boost_LIBRARY_DIRS $ENV{BOOST_LIBRARYDIR})
#endif()
#find_package(Boost COMPONENTS python)

#if( Boost_FOUND )
#  message(STATUS "found boost version: ${Boost_VERSION}")
#else()
#  message(STATUS "Could not find boost libraries!")
#endif()

add_library(otbsiftfast libsiftfast.cpp)

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
#  target_link_libraries(otbsiftfast m stdc++)
  target_link_libraries(otbsiftfast m)
endif()

#set_target_properties(otbsiftfast PROPERTIES OUTPUT_NAME  siftfast CLEAN_DIRECT_OUTPUT 1)

# compile without depending on libsiftfast
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#set_target_properties(siftfast PROPERTIES COMPILE_FLAGS "-DDVPROFILE")
#target_link_libraries(siftfast libsiftfast)

# check for OpenMP
find_package(OpenMP)
if(OPENMP_FOUND)
  message(STATUS "Enabling OpenMP support")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
else()
  message(STATUS "Disabling OpenMP support")
endif()

#Install TARGET & FILES for otb-lib
if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(otbsiftfast PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif()

install(TARGETS otbsiftfast
  EXPORT ${OTB3P_INSTALL_EXPORT_NAME}
  RUNTIME DESTINATION ${OTB3P_INSTALL_RUNTIME_DIR} COMPONENT RuntimeLibraries
  LIBRARY DESTINATION ${OTB3P_INSTALL_LIBRARY_DIR} COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION ${OTB3P_INSTALL_ARCHIVE_DIR} COMPONENT Development
  )
  
install(FILES siftfast.h
  DESTINATION ${OTB3P_INSTALL_INCLUDE_DIR}/otbsiftfast
  COMPONENT Development
  )

install(FILES otb_siftfast.h
  DESTINATION ${OTB3P_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )

otb_module_target( otbsiftfast NO_INSTALL )