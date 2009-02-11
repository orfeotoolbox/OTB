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


IF(OTB_COMPILE_JPEG2000)
  SUBDIRS( otbopenjpeg )
ENDIF(OTB_COMPILE_JPEG2000)


IF(NOT OTB_USE_EXTERNAL_EXPAT)
	SUBDIRS( otbexpat )
ENDIF(NOT OTB_USE_EXTERNAL_EXPAT)


SUBDIRS(BGL otbsvm dxflib InsightJournal otbossim otbossimplugins otb6S tinyXMLlib otbkml otbliblas otbedison otbsiftfast)

IF(BUILD_TESTING)
        SUBDIRS( Dart )
ENDIF(BUILD_TESTING)


#-----------------------------------------------------------------------------
# Install the UseOTB.cmake file configured from the top-level.
INSTALL_FILES(/lib/otb ".cmake" "OTBConfig")
