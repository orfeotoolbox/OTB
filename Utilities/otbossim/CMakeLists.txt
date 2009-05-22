PROJECT(otbossim)

SET(OSSIM_MAJOR_VERSION_NUMBER "1")
SET(OSSIM_MINOR_VERSION_NUMBER "7")
SET(OSSIM_PATCH_VERSION_NUMBER "15")
SET(OSSIM_RELEASE_NUMBER "?")
SET(OSSIM_VERSION "${OSSIM_MAJOR_VERSION_NUMBER}-${OSSIM_MINOR_VERSION_NUMBER}-${OSSIM_PATCH_VERSION_NUMBER}" )

INCLUDE(CheckIncludeFile)
CHECK_INCLUDE_FILE("dirent.h" CMAKE_HAVE_DIRENT_H)
CHECK_INCLUDE_FILE("unistd.h" CMAKE_HAVE_UNISTD_H)
CHECK_INCLUDE_FILE("getopt.h" CMAKE_HAVE_GETOPT_H)
CHECK_INCLUDE_FILE("fcntl.h" CMAKE_HAVE_FCNTL_H)
CHECK_INCLUDE_FILE("dbmalloc.h" CMAKE_HAVE_DBMALLOC_H)
CHECK_INCLUDE_FILE("malloc.h" CMAKE_HAVE_MALLOC_H)
CHECK_INCLUDE_FILE("dlfcn.h" CMAKE_HAVE_DLFCN_H)

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Utilities/otbossim/include)
INCLUDE_DIRECTORIES(${OTB_BINARY_DIR}/Utilities/otbossim/include)

CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/otbossim/include/ossim/ossimConfig.h.in
	       ${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimConfig.h)

# Run ossim/version-config.cpp taht will generate ossim/ossimVersion.h file nedeed to compile OSSIM
# SET(INCLUDES_OSSIM_TRY_RUN "-DINCLUDE_DIRECTORIES:STRING=${OTB_BINARY_DIR}/Utilities/otbossim/include" )
# TRY_RUN(RUN_RESULT_VAR COMPILE_RESULT_VAR
#         ${OTB_BINARY_DIR}
#         ${OTB_SOURCE_DIR}/Utilities/otbossim/src/ossim/version-config.cpp
#         CMAKE_FLAGS "${INCLUDES_OSSIM_TRY_RUN}"
#         OUTPUT_VARIABLE output
#         ARGS "${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimVersion.h"
# )
# IF(NOT COMPILE_RESULT_VAR)
#     MESSAGE(FATAL_ERROR "OSSIM : cannot compile version-config.cpp")
# ENDIF(NOT COMPILE_RESULT_VAR)
#
# IF(RUN_RESULT_VAR)
#     MESSAGE(FATAL_ERROR "OSSIM : cannot create ossimVersion.h file")
# ENDIF(RUN_RESULT_VAR)

ADD_EXECUTABLE(version-config ${OTB_SOURCE_DIR}/Utilities/otbossim/src/ossim/version-config.cpp)

# ADD_CUSTOM_COMMAND(
#   OUTPUT ${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimVersion.h
#   DEPENDS version-config
#   COMMAND version-config
#   ARGS "${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimVersion.h"
#   COMMENT "Generating ossimVersion.h"
#   )

# SET_SOURCE_FILES_PROPERTIES(
#   ${OTB_SOURCE_DIR}/Utilities/otbossim/src/ossim/init/ossimInit.cpp PROPERTIES
#   OBJECT_DEPENDS "${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimVersion.h"
#   )




SUBDIRS(include)
# ADDED BY OTB DEVELOPPERS TO AVOID INTENSIVE RECOMPILATION
IF(NOT OTB_DISABLE_UTILITIES_COMPILATION)
SUBDIRS(src)
ENDIF(NOT OTB_DISABLE_UTILITIES_COMPILATION)
