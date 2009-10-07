# Add an option to build or not build the FltkImageViewer Library


IF(OTB_USE_VISU_GUI)
        IF(NOT OTB_USE_EXTERNAL_FLTK)
	        ADD_SUBDIRECTORY( FLTK )
        ENDIF(NOT OTB_USE_EXTERNAL_FLTK)
ENDIF(OTB_USE_VISU_GUI)

IF(NOT OTB_USE_EXTERNAL_ITK)
  SUBDIRS(ITK)
ENDIF(NOT OTB_USE_EXTERNAL_ITK)

IF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
  SUBDIRS( otbopenthreads )
ENDIF(NOT OTB_USE_EXTERNAL_OPENTHREADS)


IF(OTB_USE_LIBLAS)
  SUBDIRS( otbliblas )
ENDIF(OTB_USE_LIBLAS)

IF(OTB_COMPILE_JPEG2000)
  SUBDIRS( otbopenjpeg )
ENDIF(OTB_COMPILE_JPEG2000)


IF(NOT OTB_USE_EXTERNAL_EXPAT)
	SUBDIRS( otbexpat )
ENDIF(NOT OTB_USE_EXTERNAL_EXPAT)

IF(NOT OTB_USE_EXTERNAL_BOOST)
  SUBDIRS(BGL)
ENDIF(NOT OTB_USE_EXTERNAL_BOOST)

SUBDIRS(otbsvm dxflib InsightJournal otbossim otbossimplugins otb6S tinyXMLlib otbkml otbedison otbsiftfast otbconfigfile)

IF(BUILD_TESTING)
        SUBDIRS( Dart )
ENDIF(BUILD_TESTING)


#-----------------------------------------------------------------------------
# Install the UseOTB.cmake file configured from the top-level.
IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES ${OTB_BINARY_DIR}/Utilities/OTBConfig.cmake
    DESTINATION ${OTB_INSTALL_PACKAGE_DIR_CM24}
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)
