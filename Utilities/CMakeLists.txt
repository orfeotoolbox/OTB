# Add an option to build or not build the FltkImageViewer Library

IF(NOT OTB_USE_EXTERNAL_ITK)
	SUBDIRS( ITK )
ENDIF(NOT OTB_USE_EXTERNAL_ITK)

IF(NOT OTB_USE_EXTERNAL_FLTK)
	SUBDIRS( FLTK )
ENDIF(NOT OTB_USE_EXTERNAL_FLTK)

SUBDIRS(BGL otbsvm dxflib InsightJournal otbossim otb6S otbgeotiff tinyXMLlib otbopenjpeg)

IF(BUILD_TESTING)
        SUBDIRS( Dart )
ENDIF(BUILD_TESTING)


#-----------------------------------------------------------------------------
# Install the UseOTB.cmake file configured from the top-level.
INSTALL_FILES(/lib/otb ".cmake" "OTBConfig")
