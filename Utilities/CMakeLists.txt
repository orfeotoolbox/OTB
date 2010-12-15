# Add an option to build or not build the FltkImageViewer Library


IF(OTB_USE_VISU_GUI)
        IF(NOT OTB_USE_EXTERNAL_FLTK)
            MESSAGE(STATUS "Configuring FLTK...")
	        ADD_SUBDIRECTORY( FLTK )
        ENDIF(NOT OTB_USE_EXTERNAL_FLTK)
ENDIF(OTB_USE_VISU_GUI)

IF(NOT OTB_USE_EXTERNAL_ITK)
  IF(NOT OTB_USE_DEPRECATED)
    SET(GDCM_LEGACY_REMOVE ON)
  ENDIF(NOT OTB_USE_DEPRECATED)
  MESSAGE(STATUS "Configuring ITK...")
  ADD_SUBDIRECTORY(ITK)
ENDIF(NOT OTB_USE_EXTERNAL_ITK)

IF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
  MESSAGE(STATUS "Configuring otbopenthreads...")
  ADD_SUBDIRECTORY( otbopenthreads )
ENDIF(NOT OTB_USE_EXTERNAL_OPENTHREADS)


IF(OTB_USE_LIBLAS)
IF(NOT OTB_USE_EXTERNAL_LIBLAS)
  MESSAGE(STATUS "Configuring otbliblas...")
  ADD_SUBDIRECTORY( otbliblas )
ENDIF(NOT OTB_USE_EXTERNAL_LIBLAS)
ENDIF(OTB_USE_LIBLAS)

IF(OTB_COMPILE_JPEG2000)
  MESSAGE(STATUS "Configuring otbopenjpeg...")
  ADD_SUBDIRECTORY( otbopenjpeg )
ENDIF(OTB_COMPILE_JPEG2000)


IF(NOT OTB_USE_EXTERNAL_EXPAT)
    MESSAGE(STATUS "Configuring otbexpat...")
	ADD_SUBDIRECTORY( otbexpat )
ENDIF(NOT OTB_USE_EXTERNAL_EXPAT)

IF(NOT OTB_USE_EXTERNAL_BOOST)
  MESSAGE(STATUS "Configuring internal Boost...")
  ADD_SUBDIRECTORY(BGL)
ENDIF(NOT OTB_USE_EXTERNAL_BOOST)

MESSAGE(STATUS "Configuring otbmuparser...")
ADD_SUBDIRECTORY(otbmuparser)
MESSAGE(STATUS "Configuring otbsvm...")
ADD_SUBDIRECTORY(otbsvm )
MESSAGE(STATUS "Configuring otbknn...")
ADD_SUBDIRECTORY(otbknn )
MESSAGE(STATUS "Configuring InsightJournal...")
ADD_SUBDIRECTORY(InsightJournal)
MESSAGE(STATUS "Configuring otbossim...")
ADD_SUBDIRECTORY(otbossim)
MESSAGE(STATUS "Configuring otbossimplugins...")
ADD_SUBDIRECTORY(otbossimplugins)
MESSAGE(STATUS "Configuring otb6S...")
ADD_SUBDIRECTORY(otb6S)
MESSAGE(STATUS "Configuring tinyXMLlib...")
ADD_SUBDIRECTORY(tinyXMLlib)
MESSAGE(STATUS "Configuring otbkml...")
ADD_SUBDIRECTORY(otbkml)
MESSAGE(STATUS "Configuring otbedison...")
ADD_SUBDIRECTORY(otbedison)
MESSAGE(STATUS "Configuring otbsiftfast...")
ADD_SUBDIRECTORY(otbsiftfast)
MESSAGE(STATUS "Configuring otbconfigfile...")
ADD_SUBDIRECTORY(otbconfigfile)

IF(BUILD_TESTING)
        ADD_SUBDIRECTORY( Dart )
ENDIF(BUILD_TESTING)


#-----------------------------------------------------------------------------
# Install the UseOTB.cmake file configured from the top-level.
IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES ${OTB_BINARY_DIR}/Utilities/OTBConfig.cmake
    DESTINATION ${OTB_INSTALL_PACKAGE_DIR_CM24}
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)
