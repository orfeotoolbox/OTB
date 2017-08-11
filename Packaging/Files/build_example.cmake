if(NOT SRC_DIR)
  message(FATAL_ERROR "SRC_DIR=${SRC_DIR}")
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
  message(FATAL_ERROR "Configure test failed.\nconfigure_ov=${configure_ov}")
  return()
endif()

execute_process(COMMAND ${MAKE_PROGRAM} HelloWorldOTB
  WORKING_DIRECTORY ${TEST_DIR}
  RESULT_VARIABLE build_HelloWorldOTB_rv
  OUTPUT_VARIABLE build_HelloWorldOTB_ov
  ERROR_VARIABLE  build_HelloWorldOTB_ov
  )

if( build_HelloWorldOTB_rv )
  message(FATAL_ERROR
    "Build test failed.\nbuild_HelloWorldOTB_ov=${build_HelloWorldOTB_ov}")
  return()
endif()


# execute_process(COMMAND ${MAKE_PROGRAM} Pipeline
#   WORKING_DIRECTORY ${TEST_DIR}
#   RESULT_VARIABLE build_Pipeline_rv
#   OUTPUT_VARIABLE build_Pipeline_ov
#   ERROR_VARIABLE  build_Pipeline_ov
#   )


# if( build_Pipeline_rv )
#   message(FATAL_ERROR
#     "Build test failed.\nbuild_Pipeline_ov=${build_Pipeline_ov}")
#   return()
# endif()
