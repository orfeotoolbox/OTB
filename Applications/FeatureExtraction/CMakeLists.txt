
OTB_CREATE_APPLICATION(NAME LineSegmentDetection
                       SOURCES otbLineSegmentDetection.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
       
OTB_CREATE_APPLICATION(NAME           RadiometricIndices
                       SOURCES        otbRadiometricIndices.cxx
                       LINK_LIBRARIES OTBIO;OTBRadiometry)
                       
OTB_CREATE_APPLICATION(NAME HaralickTextureExtraction
                       SOURCES otbHaralickTextureExtraction.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
                       
OTB_CREATE_APPLICATION(NAME SFSTextureExtraction
                       SOURCES otbSFSTextureExtraction.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME BinaryMorphologicalOperation
                       SOURCES otbBinaryMorphologicalOperation.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME GrayScaleMorphologicalOperation
                       SOURCES otbGrayScaleMorphologicalOperation.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
                       