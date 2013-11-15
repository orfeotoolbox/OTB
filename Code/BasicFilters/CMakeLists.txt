
# Sources of non-templated classes.
FILE(GLOB OTBBasicFilters_SRCS "*.cxx" )

ADD_LIBRARY(OTBBasicFilters ${OTBBasicFilters_SRCS})
TARGET_LINK_LIBRARIES (OTBBasicFilters OTBCommon otbedison ${MUPARSER_LIBRARIES})

# Explicit link to fftw is required
# The link of ITKFFT to fftw does not propagate well :
# in an external project using OTB with FFT features,
# linking to OTB libraries does not automatically add fftw to
# the link library list.
#
# Possible cause : we mix two different CMake behaviour.
# ITK uses exported targets, while OTB uses the OTBLibraryDepends file,
# and there is now a mismatch between target name and lib name for ITK libraries,
# so that only the ITK library files are linked, not considered as real target
# with their associated dependencies.
#
if(ITK_USE_FFTWD)
  if(ITK_USE_SYSTEM_FFTW)
    target_link_libraries(OTBBasicFilters ${FFTWD_LIB})
    if(FFTWD_THREADS_LIB)
      target_link_libraries(OTBBasicFilters ${FFTWD_THREADS_LIB})
    endif()
  else()
    message(FATAL "Using ITK internal version of FFTW is not supported (yet)") 
  endif()
endif()

if(ITK_USE_FFTWF)
  if(ITK_USE_SYSTEM_FFTW)
    target_link_libraries(OTBBasicFilters ${FFTWF_LIB})
    if(FFTWF_THREADS_LIB)
      target_link_libraries(OTBBasicFilters ${FFTWF_THREADS_LIB})
    endif()
  else()
    message(FATAL "Using ITK internal version of FFTW is not supported (yet)") 
  endif()
endif()

IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(OTBBasicFilters PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)

IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS OTBBasicFilters
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  INSTALL(FILES ${__files1} ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/BasicFilters
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)
