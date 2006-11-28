SUBDIRS(Common BasicFilters IO ChangeDetection FeatureExtraction Learning
MultiScale SpatialReasoning) 

IF(OTB_USE_VISU)
        SUBDIRS(Visu)
ENDIF(OTB_USE_VISU)
