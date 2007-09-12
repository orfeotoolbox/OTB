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
ADD_DEFINITIONS(-DOTB_6S_SRC)

# On Microsoft Visual
IF(WIN32)
  IF(NOT BORLAND)
    IF(NOT CYGWIN)
      IF(NOT MINGW)
#        IF(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
          ADD_DEFINITIONS(
            -DUSE_CLOCK
            -DMSDOS
            -DNO_ONEXIT
            )
#        ENDIF(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
      ELSE(NOT MINGW)
          ADD_DEFINITIONS(
            -DUSE_CLOCK
            -DMSDOS
            -DNO_TRUNCATE  #For generate endfile.c
            )
      ENDIF(NOT MINGW)
    ENDIF(NOT CYGWIN)
  ENDIF(NOT BORLAND)
ENDIF(WIN32)


# source files for otb6S
FILE(GLOB OTB6S_SRCS "6SV1.0B/*.c" )

# source files for libf2c
# Not use itkf2c because uncomplete for link 6S main exectuable
FILE(GLOB LIBF2C_SRCS "libf2c/*.c" )

LIST(REMOVE_ITEM LIBF2C_SRCS "${OTB6S_SOURCE_DIR}/libf2c/arithchk.c" )
LIST(REMOVE_ITEM LIBF2C_SRCS "${OTB6S_SOURCE_DIR}/libf2c/main.c" )
LIST(REMOVE_ITEM LIBF2C_SRCS "${OTB6S_SOURCE_DIR}/libf2c/getarg_.c" )
LIST(REMOVE_ITEM LIBF2C_SRCS "${OTB6S_SOURCE_DIR}/libf2c/iargc_.c" )



SET(FICHIER_H "${OTB_BINARY_DIR}/toto.txt")

FILE(WRITE ${FICHIER_H} "//TOTO 6S\n")

#---------Create includes
FOREACH(FILE ${LIBF2C_SRCS} )
        FILE( APPEND ${FICHIER_H} " -> ${FILE}\n" )
        
ENDFOREACH(FILE ${FILES_SOURCES} )



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


