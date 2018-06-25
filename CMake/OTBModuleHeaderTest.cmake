#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
#

# This checks HeaderTest's in each module.  A HeaderTest can be found in the
# module 'test' directory in a file otb<module_name>HeaderTest.cxx.  This
# contains a null main(), but includes all the classes in the module.  The
# primary purpose of this test is to make sure there are not missing module
# dependencies.

# This does not force the developer to install python to be able to build OTB.
# The tests will simply not be run if python is unavailable.
find_package(PythonInterp)

# The maximum number of headers in a test.  This helps limit memory issues,
# and the cppcheck tests.  However, if this is not unity, there is a slight
# chance that problems may be hidden.  For a complete header check, set to "1".
set( MAXIMUM_NUMBER_OF_HEADERS 35
  CACHE STRING "The number of headers in a HeaderTest code." )
mark_as_advanced( MAXIMUM_NUMBER_OF_HEADERS )

add_custom_target( OTBHeaderTests
  ${CMAKE_COMMAND} --build ${OTB_BINARY_DIR}
  COMMENT "Regenerating and building the header tests." )



SET(BANNED_HEADERS)
if(NOT OTB_USE_OPENCV)
	SET(BANNED_HEADERS "${BANNED_HEADERS} otbDecisionTreeMachineLearningModelFactory.h otbDecisionTreeMachineLearningModel.h otbKNearestNeighborsMachineLearningModelFactory.h otbKNearestNeighborsMachineLearningModel.h otbRandomForestsMachineLearningModelFactory.h otbRandomForestsMachineLearningModel.h otbSVMMachineLearningModelFactory.h otbSVMMachineLearningModel.h otbGradientBoostedTreeMachineLearningModelFactory.h otbGradientBoostedTreeMachineLearningModel.h otbBoostMachineLearningModelFactory.h otbBoostMachineLearningModel.h otbNeuralNetworkMachineLearningModelFactory.h otbNeuralNetworkMachineLearningModel.h otbNormalBayesMachineLearningModelFactory.h otbNormalBayesMachineLearningModel.h otbRequiresOpenCVCheck.h otbOpenCVUtils.h otbCvRTreesWrapper.h")
endif()
if(NOT OTB_USE_SHARK)
  SET(BANNED_HEADERS "${BANNED_HEADERS} otbSharkRandomForestsMachineLearningModel.h otbSharkRandomForestsMachineLearningModel.hxx otbSharkUtils.h otbRequiresSharkCheck.h otbSharkRandomForestsMachineLearningModelFactory.h  otbSharkKMeansMachineLearningModel.h otbSharkKMeansMachineLearningModel.hxx otbSharkKMeansMachineLearningModelFactory.h otbSharkKMeansMachineLearningModelFactory.hxx")
endif()
if(NOT OTB_USE_LIBSVM)
	SET(BANNED_HEADERS "${BANNED_HEADERS} otbLibSVMMachineLearningModel.h otbLibSVMMachineLearningModelFactory.h")
endif()

if(NOT OTB_USE_SIFTFAST)
  SET(BANNED_HEADERS "${BANNED_HEADERS} otbSiftFastImageFilter.h")
endif()

# ************ ADD YOUR BANNED HEADERS HERE ************
#if(CONDITION)
#	SET(BANNED_HEADERS "${BANNED_HEADERS} BANNED-HEADER1.h BANNED-HEADER2.h")
#endif()



macro( otb_module_headertest _name )
  if( NOT ${_name}_THIRD_PARTY
      AND EXISTS ${${_name}_SOURCE_DIR}/include
      AND PYTHON_EXECUTABLE
      AND NOT (PYTHON_VERSION_STRING VERSION_LESS 2.6)
      AND NOT (${_name} STREQUAL OTBTestKernel)
      AND NOT (CMAKE_GENERATOR MATCHES "^Visual Studio 10.*"))

    # Count how many tests we are going to get, and put the source files in
    # the list _outputs.
    # WARNING: This code is highly coupled with the BuildHeaderTest.py file
    # below.  Before making any logic changes here, make sure that script is not
    # effected.
    set( _include ${${_name}_SOURCE_DIR}/include )
    file( GLOB _h_files ${_include}/*.h )
    file( GLOB _hxx_files ${_include}/*.hxx )
    set( _header_files ${_h_files} ${_hxx_files} )
    list( LENGTH _h_files _num_headers )
    set( _outputs ${${_name}_BINARY_DIR}/test/${_name}HeaderTest1.cxx )
    set( _test_num 1 )
    set( _available_headers "${MAXIMUM_NUMBER_OF_HEADERS}" )
    while( ${_num_headers} GREATER ${_available_headers} )
      math( EXPR _test_num "${_test_num} + 1" )
      math( EXPR _available_headers "${_available_headers} + ${MAXIMUM_NUMBER_OF_HEADERS}" )
      list( APPEND _outputs
        ${${_name}_BINARY_DIR}/test/${_name}HeaderTest${_test_num}.cxx )
    endwhile()

    add_custom_target( ${_name}HeaderTestClean
      ${CMAKE_COMMAND} -E remove ${_outputs} )
    add_dependencies( OTBHeaderTests ${_name}HeaderTestClean )

    # We check to see if the headers are changed.  If so, remove the header test
    # source files so they are regenerated.
    if( ${CMAKE_VERSION} VERSION_GREATER 2.8.6 ) # for string( MD5
      set( _headers_list_md5 "${${_name}_BINARY_DIR}/test/CMakeFiles/HeadersList.md5" )
      list( SORT _header_files )
      string( MD5 _new_md5 "${_header_files}" )
      set( _regenerate_sources FALSE )
      if( NOT EXISTS "${_headers_list_md5}" )
        set( _regenerate_sources TRUE )
      else()
        file( READ "${_headers_list_md5}" _old_md5 )
        if( NOT ("${_old_md5}" STREQUAL "${_new_md5}"))
          set( _regenerate_sources TRUE )
        endif()
      endif()
      file( WRITE "${_headers_list_md5}" "${_new_md5}" )
      if( ${_regenerate_sources} )
        file( REMOVE ${_outputs} )
      endif()
    endif()

    set( _test_num 1 )
    foreach( _header_test_src ${_outputs} )
      get_filename_component( _test_name ${_header_test_src} NAME_WE )
      add_custom_command(
        OUTPUT ${_header_test_src}
        COMMAND ${PYTHON_EXECUTABLE} ${OTB_CMAKE_DIR}/../Utilities/Maintenance/BuildHeaderTest.py
        ${_name}
        ${${_name}_SOURCE_DIR}
        ${${_name}_BINARY_DIR}
        ${MAXIMUM_NUMBER_OF_HEADERS}
        ${_test_num}
        ${BANNED_HEADERS}
        )
      add_executable( ${_test_name} ${_header_test_src} )
      target_link_libraries( ${_test_name} OTBCommon )
      if (${_name}_LIBRARIES)
        # OTBBoostAdapters depends only on OTBBoost,
        # and OTBBoost_LIBRARIES can be empty, so check for it
        add_dependencies(${_test_name} ${${_name}_LIBRARIES})
        # this target_link should be needed only on WIN32
        target_link_libraries(${_test_name} ${${_name}_LIBRARIES})
      endif()
      add_dependencies(${_name}-all ${_test_name})
      math( EXPR _test_num "${_test_num} + 1" )
    endforeach()
  endif()
endmacro()
