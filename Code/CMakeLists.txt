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
SARPolarimetry
)

IF(OTB_USE_VISU_GUI)
        SUBDIRS(Visu Gui Visualization)
ENDIF(OTB_USE_VISU_GUI)
