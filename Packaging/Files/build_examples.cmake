if(NOT SRC_DIR)
  message(FATAL_ERROR "SRC_DIR is not set")
endif()

if(NOT MAKE_PROGRAM)
  message(FATAL_ERROR "MAKE_PROGRAM is not set")
endif()

set(PKG_DIR)

get_filename_component(PKG_DIR ${CMAKE_CURRENT_LIST_DIR} PATH)
set(TEST_DIR ${PKG_DIR}/ex_build)

if(EXISTS "${TEST_DIR}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${TEST_DIR})
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory   ${TEST_DIR})
else()
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory   ${TEST_DIR})
endif()
message("TEST_DIR=${TEST_DIR}")
message("PKG_DIR=${PKG_DIR}")
message("SRC_DIR=${SRC_DIR}")
message("MAKE_PROGRAM=${MAKE_PROGRAM}")

# if( "${CMAKE_SYSTEM_NAME}"  MATCHES "Linux")
#   find_program(GREP grep)
#   execute_process(COMMAND ${GREP} -Rs "/usr/"
#     WORKING_DIRECTORY ${PKG_DIR}/lib/cmake/
#     RESULT_VARIABLE grep_usr_rv
#     OUTPUT_VARIABLE grep_usr_ov
#     ERROR_VARIABLE  grep_usr_ov
#     )

#   if(NOT grep_usr_rv EQUAL 1)
#     message(FATAL_ERROR
#       "Test 'grep_usr_rv' failed with output:\n${grep_usr_ov}")
#     return()
#   endif()
# endif() #Linux

execute_process(
  COMMAND ${CMAKE_COMMAND}
  -DCMAKE_INSTALL_PREFIX=${PKG_DIR}
  ${SRC_DIR}
  WORKING_DIRECTORY ${TEST_DIR}
  RESULT_VARIABLE configure_rv
  OUTPUT_VARIABLE configure_ov
  ERROR_VARIABLE  configure_ov
  )

if( configure_rv )
  message(FATAL_ERROR "Configure FAILED. configure_ov:\n${configure_ov}")
  return()
else()
  message("Configure PASSED. configure_ov:\n${configure_ov}")
endif()

# If you want to test building a third targert, use foreach.
# the code is written in a way that it is easy to stuff these two
# into a cmake foreach
execute_process(COMMAND ${MAKE_PROGRAM} HelloWorldOTB
  WORKING_DIRECTORY ${TEST_DIR}
  RESULT_VARIABLE build_HelloWorldOTB_rv
  OUTPUT_VARIABLE build_HelloWorldOTB_ov
  ERROR_VARIABLE  build_HelloWorldOTB_ov
  )

if( build_HelloWorldOTB_rv )
  message(FATAL_ERROR "Build FAILED. build_HelloWorldOTB_ov:\n${build_HelloWorldOTB_ov}")
  return()
else()
  message("Build PASSED. build_HelloWorldOTB_ov:\n${build_HelloWorldOTB_ov}")
endif()

execute_process(COMMAND ${MAKE_PROGRAM} Pipeline
  WORKING_DIRECTORY ${TEST_DIR}
  RESULT_VARIABLE build_Pipeline_rv
  OUTPUT_VARIABLE build_Pipeline_ov
  ERROR_VARIABLE  build_Pipeline_ov
  )

if( build_Pipeline_rv )
  message(FATAL_ERROR "Build FAILED. build_Pipeline_ov=${build_Pipeline_ov}")
  return()
else()
  message("Build PASSED. build_Pipeline_ov=${build_Pipeline_ov}")  
endif()
