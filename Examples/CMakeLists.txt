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
)

IF(OTB_USE_VISU)
        SUBDIRS(Visu)
ENDIF(OTB_USE_VISU)

IF(OTB_USE_PATENTED)
  SUBDIRS( Patented )
ENDIF(OTB_USE_PATENTED)

