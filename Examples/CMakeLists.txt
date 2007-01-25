# $Id$

SUBDIRS( 
  BasicFilters
  FeatureExtraction
  DataRepresentation
  IO
  Filtering
  ChangeDetection
  Learning
  Classification
  Segmentation
  Iterators
  MultiScale
)

IF(OTB_USE_VISU)
        SUBDIRS(Visu)
ENDIF(OTB_USE_VISU)

IF(OTB_USE_PATENTED)
  SUBDIRS( Patented )
ENDIF(OTB_USE_PATENTED)

#Recopie du fichier README.txt dans l'arborescence BINARY
IF( EXISTS ${OTB_BINARY_DIR}/Examples/README.txt )
ELSE( EXISTS ${OTB_BINARY_DIR}/Examples/README.txt )
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Examples/README.txt
    ${OTB_BINARY_DIR}/Examples/README.txt @ONLY IMMEDIATE)
ENDIF( EXISTS ${OTB_BINARY_DIR}/Examples/README.txt )
