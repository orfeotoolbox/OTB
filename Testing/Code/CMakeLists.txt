# $Id$

SUBDIRS(
Common 
IO 
BasicFilters 
FeatureExtraction 
ChangeDetection 
Learning 
MultiScale 
DisparityMap 
SpatialReasoning
Projections
Radiometry
Fusion
Markov
SARPolarimetry
)

IF(OTB_USE_VISU_GUI)
        SUBDIRS(Visu Gui)
ENDIF(OTB_USE_VISU_GUI)
