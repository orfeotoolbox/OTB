INCLUDE_REGULAR_EXPRESSION("^.*$")
# Defines the source code for the library
SET(OPENJPEG_SRCS
  tcd.c
  bio.c
  cio.c
  dwt.c
  event.c
  image.c
  j2k.c
  j2k_lib.c
  jp2.c
  jpt.c
  mct.c
  mqc.c
  openjpeg.c
  pi.c
  raw.c
  t1.c
  tgt.c
  profile.c
  invert.c
  function_list.c
  t2.c
)

# disable gcc warnings
IF(CMAKE_COMPILER_IS_GNUCC)
  FOREACH(f ${OPENJPEG_SRCS})
        SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS -w )
  ENDFOREACH(f)
ENDIF(CMAKE_COMPILER_IS_GNUCC)

# Pass proper definition to preprocessor to generate shared lib

IF(OTB_BUILD_SHARED_LIBS)
 ADD_DEFINITIONS(-DOPJ_EXPORTS)
ELSE(BUILD_SHARED_LIBS)
 ADD_DEFINITIONS(-DOPJ_STATIC)
ENDIF(OTB_BUILD_SHARED_LIBS)


IF(ENABLE_PROFILING)
    ADD_DEFINITIONS(-D_PROFILE)
ENDIF(ENABLE_PROFILING)

ADD_DEFINITIONS(-DUSE_OPJ_DEPRECATED)

# Create the library
ADD_LIBRARY(${OPENJPEG_LIBRARY_NAME} ${OPENJPEG_SRCS})
SET_TARGET_PROPERTIES(${OPENJPEG_LIBRARY_NAME} PROPERTIES
	${OPENJPEG_LIBRARY_PROPERTIES})

# Install library
INSTALL(TARGETS ${OPENJPEG_LIBRARY_NAME}
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)


# Install includes files
INSTALL(FILES openjpeg.h
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbopenjpeg/libopenjpeg 
    COMPONENT Development)
