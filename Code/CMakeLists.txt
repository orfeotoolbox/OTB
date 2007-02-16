SUBDIRS(Common BasicFilters IO ChangeDetection FeatureExtraction Learning MultiScale DisparityMap SpatialReasoning) 

IF(OTB_USE_VISU)
        SUBDIRS(Visu Gui)
ENDIF(OTB_USE_VISU)
