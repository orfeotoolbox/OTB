# This file contains CMake functions and macros used when testing ITK modules.

#-----------------------------------------------------------------------------
# Create source code, compile and link a test driver
# Two variables must be defined before including this file.
#   KIT should define a unique name for the test driver.
#   KitTests should contain a list of test file names.
# Arguments - Input
#   KIT - the name of the test directory
#   KIT_LIBS - a list of libraries needed to link the test driver
#   KitTests - a list of tests to be included in the test driver
#   ADDITIONAL_SRC (optional) - additional source files, which don't contain tests

macro(CreateTestDriver KIT KIT_LIBS KitTests)
  set(ADDITIONAL_SRC ${ARGN})
  if(EMSCRIPTEN)
    set(emscripten_before "
EM_ASM(
  var cmake_source_dir = '${CMAKE_SOURCE_DIR}'.split('/');
  // This is intentionally global so it can be unmounted at the end.
  source_mount_dir = null;
  if(cmake_source_dir[1] === 'home') {
    source_mount_dir = cmake_source_dir.slice(0, 3).join('/');
    }
  else {
    source_mount_dir = cmake_source_dir.slice(0, 2).join('/');
    }
  FS.mkdir(source_mount_dir);
  FS.mount(NODEFS, { root: source_mount_dir }, source_mount_dir);

  var cmake_binary_dir = '${CMAKE_BINARY_DIR}'.split('/');
  // This is intentionally global so it can be unmounted at the end.
  binary_mount_dir = null;
  if(cmake_binary_dir[1] === 'home') {
    binary_mount_dir = cmake_binary_dir.slice(0, 3).join('/');
    }
  else {
    binary_mount_dir = cmake_binary_dir.slice(0, 2).join('/');
    }
  if(source_mount_dir != binary_mount_dir) {
    FS.mkdir(binary_mount_dir);
    FS.mount(NODEFS, { root: binary_mount_dir }, binary_mount_dir);
    }
  );
")
    set(emscripten_after "
EM_ASM(
  FS.unmount(source_mount_dir);
  if(source_mount_dir != binary_mount_dir) {
    FS.unmount(binary_mount_dir);
    }
  );
")
  endif()
  set(CMAKE_TESTDRIVER_BEFORE_TESTMAIN "${emscripten_before}#include \"otbTestDriverBeforeTest.inc\"")
  set(CMAKE_TESTDRIVER_AFTER_TESTMAIN "#include \"otbTestDriverAfterTest.inc\"${emscripten_after}")
  create_test_sourcelist(Tests ${KIT}TestDriver.cxx
    ${KitTests}
    EXTRA_INCLUDE otbTestDriverIncludeRequiredIOFactories.h
    FUNCTION  ProcessArgumentsAndRegisterRequiredFactories
    )
  add_executable(${KIT}TestDriver ${KIT}TestDriver.cxx ${Tests} ${ADDITIONAL_SRC})
  target_link_libraries(${KIT}TestDriver LINK_PUBLIC ${KIT_LIBS} ${OTBTestKernel_LIBRARIES})
  otb_module_target_label(${KIT}TestDriver)
endmacro()


macro(CreateTestDriver_SupportBuildInIOFactories KIT KIT_LIBS KitTests)
  set(ADDITIONAL_SRC ${ARGN} )
  if(EMSCRIPTEN)
    set(emscripten_before "
EM_ASM(
  var cmake_source_dir = '${CMAKE_SOURCE_DIR}'.split('/');
  // This is intentionally global so it can be unmounted at the end.
  source_mount_dir = null;
  if(cmake_source_dir[1] === 'home') {
    source_mount_dir = cmake_source_dir.slice(0, 3).join('/');
    }
  else {
    source_mount_dir = cmake_source_dir.slice(0, 2).join('/');
    }
  FS.mkdir(source_mount_dir);
  FS.mount(NODEFS, { root: source_mount_dir }, source_mount_dir);

  // This is intentionally global so it can be unmounted at the end.
  binary_mount_dir = null;
  var cmake_binary_dir = '${CMAKE_BINARY_DIR}'.split('/');
  if(cmake_binary_dir[1] === 'home') {
    binary_mount_dir = cmake_binary_dir.slice(0, 3).join('/');
    }
  else {
    binary_mount_dir = cmake_binary_dir.slice(0, 2).join('/');
    }
  if(source_mount_dir != binary_mount_dir) {
    FS.mkdir(binary_mount_dir);
    FS.mount(NODEFS, { root: binary_mount_dir }, binary_mount_dir);
    }
  );
")
    set(emscripten_after "
EM_ASM(
  FS.unmount(source_mount_dir);
  if(source_mount_dir != binary_mount_dir) {
    FS.unmount(binary_mount_dir);
    }
  );
")
  endif()
  set(CMAKE_TESTDRIVER_BEFORE_TESTMAIN "${emscripten_before}#include \"otbTestDriverBeforeTest.inc\"")
  set(CMAKE_TESTDRIVER_AFTER_TESTMAIN "#include \"otbTestDriverAfterTest.inc\"${emscripten_after}")
  create_test_sourcelist(Tests ${KIT}TestDriver.cxx
    ${KitTests}
    EXTRA_INCLUDE  otbTestDriverIncludeBuiltInIOFactories.h
    FUNCTION  ProcessArgumentsAndRegisterBuiltInFactories
    )
  add_executable(${KIT}TestDriver ${KIT}TestDriver.cxx ${Tests} ${ADDITIONAL_SRC})
  target_link_libraries(${KIT}TestDriver LINK_PUBLIC ${KIT_LIBS} ${OTBTestKernel_LIBRARIES})
  otb_module_target_label(${KIT}TestDriver)
endmacro()

#-----------------------------------------------------------------------------
# OTB wrapper for add_test that automatically sets the test's LABELS property
# to the value of its containing module.
#
# function(otb_add_test)
#   # Add tests with data in the OTBData group.
#   ExternalData_add_test(OTBData ${ARGN})

#   if("NAME" STREQUAL "${ARGV0}")
#     set(_iat_testname ${ARGV1})
#   else()
#     set(_iat_testname ${ARGV0})
#   endif()

#   if(otb-module)
#     set(_label ${otb-module})
#     if(TARGET ${otb-module}-all AND "${ARGN}" MATCHES "DATA{")
#       add_dependencies(${otb-module}-all OTBData)
#     endif()
#   else()
#     set(_label ${main_project_name})
#   endif()

#   set_property(TEST ${_iat_testname} PROPERTY LABELS ${_label})
# endfunction()

#-----------------------------------------------------------------------------
# OTB wrapper for add_test that automatically sets the test's LABELS property
# to the value of its containing module.
#
function(otb_add_test)
  set(largeinput_regex "LARGEINPUT{([^;{}\r\n]*)}")

  set(_depends_on_largeinput OFF)
  foreach(arg IN LISTS ARGN)
     if("x${arg}" MATCHES "${largeinput_regex}")
       string(REGEX REPLACE "${largeinput_regex}" "\\1" largeinput_relative_path "${arg}")
       set(_fullpath "${OTB_DATA_LARGEINPUT_ROOT}/${largeinput_relative_path}")
       list(APPEND _out_arg ${_fullpath})
       set(_depends_on_largeinput ON)
     else()
       list(APPEND _out_arg ${arg})
     endif()
  endforeach()

  if (_depends_on_largeinput AND NOT OTB_DATA_USE_LARGEINPUT)
    return()
  endif()

  add_test(${_out_arg})

  if("NAME" STREQUAL "${ARGV0}")
    set(_iat_testname ${ARGV1})
  else()
    set(_iat_testname ${ARGV0})
  endif()

  if(otb-module)
    set(_label ${otb-module})
  else()
    set(_label ${main_project_name})
  endif()

  set_property(TEST ${_iat_testname} PROPERTY LABELS ${_label})

endfunction()

#-----------------------------------------------------------------------------
# OTB function to ignore a test
#
function(otb_tests_ignore)
  set_property(GLOBAL APPEND PROPERTY CTEST_CUSTOM_TESTS_IGNORE ${ARGN})
endfunction()

#-----------------------------------------------------------------------------
# OTB function to ignore a test during MemCheck
#
function(otb_memcheck_ignore)
  set_property(GLOBAL APPEND PROPERTY CTEST_CUSTOM_MEMCHECK_IGNORE ${ARGN})
endfunction()

#-----------------------------------------------------------------------------
# Function otb_add_test_mpi to run mpi tests
function(otb_add_test_mpi)
   set( _OPTIONS_ARGS )
   set( _ONE_VALUE_ARGS NAME NBPROCS COMMAND)
   set( _MULTI_VALUE_ARGS )
   cmake_parse_arguments( TEST_MPI "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN} )

   # Test nb procs
   if( NOT TEST_MPI_NBPROCS )
     set(TEST_MPI_NBPROCS 2)
   endif()
   # Test command line
   foreach(arg IN LISTS TEST_MPI_UNPARSED_ARGUMENTS)
     list(APPEND ARGS ${arg})
   endforeach()
   set (test_parameters -np ${TEST_MPI_NBPROCS} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_MPI_COMMAND} ${ARGS})
   otb_add_test(NAME ${TEST_MPI_NAME} COMMAND ${MPIEXEC} ${test_parameters})
endfunction()
