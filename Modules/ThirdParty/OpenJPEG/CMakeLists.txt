project(OTBOpenJPEG)
  
  set(OTBOpenJPEG_SYSTEM_INCLUDE_DIRS ${OPENJPEG_INCLUDE_DIRS})
  set(OTBOpenJPEG_LIBRARIES "${OPENJPEG_LIBRARIES}")
  set(OTBOpenJPEG_INCLUDE_DIRS ${OTBOpenJPEG_BINARY_DIR}/src)
  set(OTBOpenJPEG_EXPORT_CODE_BUILD "
if(NOT OpenJPEG_FOUND)
find_package(OpenJPEG REQUIRED HINTS ${OpenJPEG_DIR})
endif()")
  set(OTBOpenJPEG_EXPORT_CODE_INSTALL ${OTBOpenJPEG_EXPORT_CODE_BUILD})
  
  if(OpenJPEG_FOUND)
    math(EXPR OTBOpenJPEG_VERSION_NUMBER
      "((${OPENJPEG_MAJOR_VERSION})*100+${OPENJPEG_MINOR_VERSION})*100+${OPENJPEG_BUILD_VERSION}")
    configure_file( src/otb_openjpeg.h.in src/otb_openjpeg.h )
  endif()

  message(STATUS "Found OpenJPEG : ${OPENJPEG_LIBRARIES} (version ${OPENJPEG_MAJOR_VERSION}.${OPENJPEG_MINOR_VERSION}.${OPENJPEG_BUILD_VERSION})")
  message(STATUS  "Caution : there could be a version conflict if ITK also contains an OpenJPEG version. "
                  "Depending on ITK version and enabled modules, the OpenJPEG headers can be "
                  "present in ITK include folder. Since the include order between modules is "
                  "deterministic, the OpenJPEG header from ITK may be included first. To avoid "
                  "version conflicts, try to use an ITK installed without GDCM nor ITKReview. "
                  "An alternative fix is to simply hide the header openjpeg.h in the ITK "
                  "include directory.")

  otb_module_impl()

install(FILES ${OTBOpenJPEG_BINARY_DIR}/src/otb_openjpeg.h
  DESTINATION ${OTBOpenJPEG_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
