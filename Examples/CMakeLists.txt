# $Id$

SUBDIRS( 
  BasicFilters
  FeatureExtraction
  DataRepresentation
  IO
  Filtering
  ChangeDetection
  Visu
  Learning
  Classification
  Segmentation
  Iterators
)

IF(OTB_USE_PATENTED)
  SUBDIRS( Patented )
ENDIF(OTB_USE_PATENTED)

