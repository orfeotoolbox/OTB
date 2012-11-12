
OTB_CREATE_APPLICATION(NAME LineSegmentDetection
                       SOURCES otbLineSegmentDetection.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
       
OTB_CREATE_APPLICATION(NAME           RadiometricIndices
                       SOURCES        otbRadiometricIndices.cxx
                       LINK_LIBRARIES OTBIO;OTBRadiometry)
                       
OTB_CREATE_APPLICATION(NAME HaralickTextureExtraction
                       SOURCES otbHaralickTextureExtraction.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME HomologousPointsExtraction
                       SOURCES otbHomologousPointsExtraction.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters;OTBFeatureExtraction)
                       
OTB_CREATE_APPLICATION(NAME SFSTextureExtraction
                       SOURCES otbSFSTextureExtraction.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME BinaryMorphologicalOperation
                       SOURCES otbBinaryMorphologicalOperation.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME GrayScaleMorphologicalOperation
                       SOURCES otbGrayScaleMorphologicalOperation.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME LocalStatisticExtraction
                       SOURCES otbLocalStatisticExtraction.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters;OTBFeatureExtraction)
                       