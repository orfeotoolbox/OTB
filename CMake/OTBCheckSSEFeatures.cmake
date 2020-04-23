#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# This should be deleted as it can be done with cmake_host_system_information
# https://cmake.org/cmake/help/v3.10/command/cmake_host_system_information.html?highlight=sse
function(check_sse_features sse_flags )
  include(CheckCXXSourceRuns)
  include(CheckCXXCompilerFlag)

  set(${sse_flags} "0" PARENT_SCOPE)
  # For apple assume sse2 is on for all intel builds, check for 64 and 32 bit versions
  if(APPLE)
      message(STATUS "For apple assume sse2 is on for all intel builds")
    return()
  endif()
  set(sse_flags_detected)
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
    set(CMAKE_REQUIRED_FLAGS "-msse2")
    set(sse_flags_detected "-msse2 -mfpmath=sse")
  elseif(MSVC AND NOT CMAKE_CL_64)
    set(CMAKE_REQUIRED_FLAGS "/arch:SSE2")
    set(sse_flags_detected "/arch:SSE2")
  endif()

  check_cxx_source_runs("
    #include <emmintrin.h>
    int main ()
    {
     __m128d a, b;
     double vals[2] = {0};
     a = _mm_loadu_pd (vals);
     b = _mm_add_pd (a,a);
     _mm_storeu_pd (vals,b);
     return (0);
    }"
    HAVE_SSE2_EXTENSIONS)

  if(HAVE_SSE2_EXTENSIONS)
    set(${sse_flags} "${sse_flags_detected}" PARENT_SCOPE)
    return()
  endif()

  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
    set(CMAKE_REQUIRED_FLAGS "-msse")
    set(sse_flags_detected "-msse -mfpmath=sse")
  elseif(MSVC AND NOT CMAKE_CL_64)
    set(CMAKE_REQUIRED_FLAGS "/arch:SSE")
    set(sse_flags_detected "/arch:SSE")
  endif()

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
    }" HAVE_SSE_EXTENSIONS)

  if(HAVE_SSE_EXTENSIONS)
    set(${sse_flags} "${sse_flags_detected}" PARENT_SCOPE)
    return()
  endif()
endfunction() #check_sse_features
