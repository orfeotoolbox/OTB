
# Sources of non-templated classes.
file(GLOB OTBBasicFilters_SRCS "*.cxx" )

add_library(OTBBasicFilters ${OTBBasicFilters_SRCS})
target_link_libraries(OTBBasicFilters OTBCommon otbedison ${MUPARSER_LIBRARIES})

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

if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBBasicFilters PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif()

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBBasicFilters
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif()

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  install(FILES ${__files1} ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/BasicFilters
    COMPONENT Development)
endif()
