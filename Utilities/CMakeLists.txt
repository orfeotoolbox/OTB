# Add an option to build or not build the FltkImageViewer Library


IF(NOT OTB_USE_EXTERNAL_ITK)
  SUBDIRS(ITK)
  # Case for release 2.2.1
  IF(NOT OTB_USE_OPENJPEG)
    SUBDIRS( ITK otbopenjpeg)
  ENDIF(NOT OTB_USE_OPENJPEG)
ENDIF(NOT OTB_USE_EXTERNAL_ITK)

IF(OTB_USE_VISU_GUI)
        IF(NOT OTB_USE_EXTERNAL_FLTK)
	        SUBDIRS( FLTK )
        ENDIF(NOT OTB_USE_EXTERNAL_FLTK)
ENDIF(OTB_USE_VISU_GUI)

IF(NOT OTB_USE_EXTERNAL_EXPAT)
	#SUBDIRS( otbexpat )
ENDIF(NOT OTB_USE_EXTERNAL_EXPAT)

#Supress libraries not used by the 2.2.0 version 
#SUBDIRS(BGL otbsvm dxflib InsightJournal otbossim otb6S otbgeotiff tinyXMLlib otbgalib otbkml)
SUBDIRS(BGL otbsvm dxflib InsightJournal otbossim otb6S otbgeotiff tinyXMLlib)

IF(BUILD_TESTING)
        SUBDIRS( Dart )
ENDIF(BUILD_TESTING)


#-----------------------------------------------------------------------------
# Install the UseOTB.cmake file configured from the top-level.
INSTALL_FILES(/lib/otb ".cmake" "OTBConfig")
