#TravisBuild.cmake - custom cmake scripts for travis build
#before running this script you need to set the below env variables
set(ENV{LC_ALL} C)
set(CTEST_PROJECT_NAME "OTB")
set(CTEST_BUILD_CONFIGURATION Release)
if(NOT DEFINED CTEST_GIT_COMMAND)
  find_program(CTEST_GIT_COMMAND NAMES git git.cmd)
endif()
# Just check if required variables are defined.
foreach(req
    CTEST_DASHBOARD_ROOT
    CMAKE_MAKE_PROGRAM
    CTEST_CMAKE_GENERATOR
    OTB_INSTALL_DIR
    XDK_INSTALL_DIR
    CTEST_COMMAND
    )
  if(NOT DEFINED ENV{${req}})
    message(FATAL_ERROR "you must set ${req}  environment variable is not defined")
  endif()
  set(${req} "$ENV{${req}}")
endforeach()

set(BUILD_NAME_PREFIX "travis-$ENV{TRAVIS_BUILD_NUMBER}")
if(NOT "$ENV{TRAVIS_PULL_REQUEST}" MATCHES "false")
  set(BUILD_NAME_PREFIX "PR-travis-$ENV{TRAVIS_PULL_REQUEST}")
endif()

set(CTEST_BUILD_NAME "${BUILD_NAME_PREFIX}-$ENV{CC}-${CTEST_BUILD_CONFIGURATION}-$ENV{TRAVIS_BRANCH}")
set(CTEST_SITE "travis-ci.org")

set(CTEST_SOURCE_DIRECTORY "${CTEST_DASHBOARD_ROOT}/orfeotoolbox/OTB")
set(CTEST_BINARY_DIRECTORY "${CTEST_DASHBOARD_ROOT}/orfeotoolbox/build")

set(CTEST_CUSTOM_MAXIMUM_NUMBER_OF_ERRORS 1000)
set(CTEST_CUSTOM_MAXIMUM_NUMBER_OF_WARNINGS 300)
#set(CTEST_BUILD_FLAGS -j2)

macro(do_submit)
  ctest_submit()
  return()
endmacro()

message(STATUS "CTEST_SOURCE_DIRECTORY: ${CTEST_SOURCE_DIRECTORY}")
message(STATUS "CTEST_BINARY_DIRECTORY: ${CTEST_BINARY_DIRECTORY}")

set(INITIAL_CACHE
"
BUILD_TESTING:BOOL=ON
CMAKE_INSTALL_PREFIX:PATH=${OTB_INSTALL_DIR}
CMAKE_MAKE_PROGRAM:FILEPATH=${CMAKE_MAKE_PROGRAM}
OTB_USE_OPENCV:BOOL=ON
OTB_USE_LIBSVM:BOOL=ON
OTB_USE_MUPARSER:BOOL=ON
OTB_USE_MUPARSERX:BOOL=ON
OTB_USE_QT4:BOOL=ON
OTB_USE_OPENGL:BOOL=ON
OTB_USE_GLEW:BOOL=ON
CMAKE_C_FLAGS:STRING=-Wextra
CMAKE_CXX_FLAGS:STRING=-Wextra -Wno-gnu-static-float-init -Wno-\\#warnings
CMAKE_BUILD_TYPE=${CTEST_BUILD_CONFIGURATION}
QT_INSTALL_TRANSLATIONS:PATH=${XDK_INSTALL_DIR}/translations
QT_MOC_EXECUTABLE:FILEPATH=${XDK_INSTALL_DIR}/bin/moc
QT_UIC_EXECUTABLE:FILEPATH=${XDK_INSTALL_DIR}/bin/uic
QT_RCC_EXECUTABLE:FILEPATH=${XDK_INSTALL_DIR}/bin/rcc
QT_INSTALL_PLUGINS:PATH=${XDK_INSTALL_DIR}/plugins
QT_INSTALL_HEADERS:PATH=${XDK_INSTALL_DIR}/include
QMAKE_MKSPECS:PATH=${XDK_INSTALL_DIR}/mkspecs
"

)
file(WRITE "${CTEST_BINARY_DIRECTORY}/CMakeCache.txt" ${INITIAL_CACHE})

ctest_start     (Continuous)
ctest_update    (SOURCE ${CTEST_SOURCE_DIRECTORY} RETURN_VALUE _update_rv)

message(STATUS "Found ${_update_rv} changed files")

ctest_configure (BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE _configure_rv)
ctest_read_custom_files(${CTEST_BINARY_DIRECTORY})


if(NOT _configure_rv EQUAL 0)
  do_submit()
endif()

ctest_build     (BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE _build_rv)

if(NOT _build_rv EQUAL 0)
  do_submit()
endif ()

execute_process(COMMAND  ${CTEST_COMMAND} "-R" "Tu" "."
  WORKING_DIRECTORY "${CTEST_BINARY_DIRECTORY}"
  )

do_submit()
