# $Id$

SUBDIRS(Common IO BasicFilters FeatureExtraction ChangeDetection Learning MultiScale DisparityMap SpatialReasoning)

IF(OTB_USE_VISU)
        SUBDIRS(Visu Gui)
ENDIF(OTB_USE_VISU)
