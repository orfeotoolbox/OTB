# $Id$

SUBDIRS(Common IO BasicFilters FeatureExtraction ChangeDetection Learning
MultiScale)

IF(OTB_USE_VISU)
        SUBDIRS(Visu)
ENDIF(OTB_USE_VISU)
