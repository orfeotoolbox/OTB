# Add an option to build or not build the FltkImageViewer Library

SUBDIRS( CAI )

IF(NOT OTB_USE_EXTERNAL_ITK)
	SUBDIRS( ITK )
ENDIF(NOT OTB_USE_EXTERNAL_ITK)


