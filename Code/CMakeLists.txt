SUBDIRS(
Common 
BasicFilters 
IO 
ChangeDetection 
FeatureExtraction 
Learning 
MultiScale 
DisparityMap 
SpatialReasoning
Projections
Radiometry
Fusion
Markov
RADAR
) 

IF(OTB_USE_VISU_GUI)
        SUBDIRS(Visu Gui)
ENDIF(OTB_USE_VISU_GUI)
