#
# Current informations:
# Version 1.0B released on May, 2005.
# Download site : http://6s.ltdri.org/
#

PROJECT(OTB6S C)

# Allow sources in subdirectories to see the include files.
INCLUDE_DIRECTORIES(${OTB6S_SOURCE_DIR})

# Inform header files they are being included from a source inside the
# otb_6S library.
#ADD_DEFINITIONS(-DOTB_6S_SRC)


# source files for otb6S
FILE(GLOB OTB6S_SRCS "6SV1.0B/*.c" )

# source files for libf2c
# Not use itkf2c because uncomplete for link 6S main exectuable
FILE(GLOB LIBF2C_SRCS "libf2c/*.c" )


INCLUDE_DIRECTORIES(${OTB6S_SOURCE_DIR}/libf2c)

# Adjust the compiler flags to avoid problems with f2c-generated code.
IF(CMAKE_COMPILER_IS_GNUCC)
  # Hide warnings in f2c-generated code.
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")

  # CMake versions before 2.4 did not put the source-file-specific flags
  # after the directory-level flags.  Remove optimization flags from the
  # CMAKE_C_FLAGS* variables so they can be overridden.
  IF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.4)
    FOREACH(c "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
      STRING(REGEX REPLACE
        "-O[^ ]*( |$)" "" CMAKE_C_FLAGS${c} "${CMAKE_C_FLAGS${c}}")
    ENDFOREACH(c)
  ENDIF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.4)

ENDIF(CMAKE_COMPILER_IS_GNUCC)


ADD_LIBRARY(otb6S ${LIBF2C_SRCS} ${OTB6S_SRCS} )

#TARGET_LINK_LIBRARIES (otb6S f2c)
IF(UNIX)
    TARGET_LINK_LIBRARIES (otb6S m)
ENDIF(UNIX)

INSTALL(TARGETS otb6S
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)


FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/6SV1.0B/*.h")
FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/libf2c/*.h")

INSTALL(FILES __files1
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otb6S/6SV1.0B
    COMPONENT Development)
INSTALL(FILES __files2
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otb6S/libf2c
    COMPONENT Development)


