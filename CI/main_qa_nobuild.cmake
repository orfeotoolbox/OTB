set (ENV{LANG} "C") # Only ascii output
get_filename_component(OTB_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)


set( QA ON )
set( ci_build_type "Debug" )
set( CTEST_TEST_TIMEOUT 1500)
set ( DEBUG "1" )
set (CMAKE_COMMAND "cmake")
set ( ci_skip_build ON)
set ( ci_skip_install ON)
set ( ci_skip_testing ON)

include( "${CMAKE_CURRENT_LIST_DIR}/main_ci.cmake" )
