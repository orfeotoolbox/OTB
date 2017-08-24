# add_test(NAME <name> COMMAND <command> [<arg>...]
#          [CONFIGURATIONS <config>...]
#          [WORKING_DIRECTORY <dir>])

set(testing_dir ${CMAKE_BINARY_DIR}/tests)
if(EXISTS "${testing_dir}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${testing_dir})
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory   ${testing_dir})
else()
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory   ${testing_dir})
endif()

set(install_package_ARGS)
set(selftester_ARGS)
set(pkg_extracted_dir "${testing_dir}/${PKG_STAGE_DIR}")
if(WIN32)
  set(install_package_ARGS "x;${PACKAGE_OUTPUT_FILE}")
  set(install_package_CMD "${ZIP_EXECUTABLE}")
  #selftester
  set(selftester_CMD ${pkg_extracted_dir}/tools/selftester.bat)
  set(selftester_ARGS "/q")
  #uninstall OTB
  set(uninstall_otb_CMD ${pkg_extracted_dir}/tools/uninstall_otb.bat)
else()
  set(install_package_ARGS "--noprogress;--nox11;--target;${pkg_extracted_dir}")
  set(install_package_CMD "${PACKAGE_OUTPUT_FILE}")
  #selftester
  set(selftester_CMD ${pkg_extracted_dir}/tools/selftester.sh)
  set(selftester_ARGS)
  #uninstall OTB
  set(uninstall_otb_CMD ${pkg_extracted_dir}/tools/uninstall_otb.sh)
endif()
add_test(
  NAME Tu_install_package
  COMMAND ${install_package_CMD} ${install_package_ARGS}
  WORKING_DIRECTORY ${testing_dir}
  )

add_test(
  NAME Tu_selftester
  COMMAND ${selftester_CMD} ${selftester_ARGS}
  WORKING_DIRECTORY ${pkg_extracted_dir}
  )

add_test(
  NAME Tu_build_examples
  COMMAND ${CMAKE_COMMAND}
  -DSRC_DIR=${CMAKE_SOURCE_DIR}/../Examples
  -DMAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
  -P ${pkg_extracted_dir}/tools/build_examples.cmake
  WORKING_DIRECTORY ${testing_dir}
  )

add_test(
  NAME Tu_uninstall_otb
  COMMAND ${uninstall_otb_CMD}
  WORKING_DIRECTORY ${testing_dir}
  )

#configure test dependencies..
set_tests_properties ( Tu_selftester PROPERTIES DEPENDS Tu_install_package)
set_tests_properties ( Tu_build_examples PROPERTIES DEPENDS Tu_selftester)
set_tests_properties ( Tu_uninstall_otb PROPERTIES DEPENDS Tu_build_examples)

#if(UNIX)
  #TODO: add test with shellcheck for script files
#endif()
