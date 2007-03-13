# Add an option to build or not build the FltkImageViewer Library

IF(NOT OTB_USE_EXTERNAL_ITK)
	SUBDIRS( ITK )
ENDIF(NOT OTB_USE_EXTERNAL_ITK)

SUBDIRS( BGL otbsvm)

OPTION(OTB_OSSIM_COMPILATION "EXPERIMENTAL: Compile OSSIM library embedded within OTB" OFF)

IF(OTB_OSSIM_COMPILATION)
SUBDIRS(OSSIM)
ENDIF(OTB_OSSIM_COMPILATION) 

IF(BUILD_TESTING)
        SUBDIRS( Dart )
ENDIF(BUILD_TESTING)


#-----------------------------------------------------------------------------
# Install the UseOTB.cmake file configured from the top-level.
INSTALL_FILES(/lib/otb ".cmake" "OTBConfig")
