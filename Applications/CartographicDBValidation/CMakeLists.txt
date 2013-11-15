OTB_CREATE_APPLICATION(NAME           ComputePolylineFeatureFromImage
                       SOURCES        otbComputePolylineFeatureFromImage.cxx
                       LINK_LIBRARIES OTBBasicFilters OTBLearning)

OTB_CREATE_APPLICATION(NAME           DSFuzzyModelEstimation
                       SOURCES        otbDSFuzzyModelEstimation.cxx
                       LINK_LIBRARIES OTBIO OTBFeatureExtraction OTBFuzzy)

OTB_CREATE_APPLICATION(NAME           VectorDataDSValidation
                       SOURCES        otbVectorDataDSValidation.cxx
                       LINK_LIBRARIES OTBIO OTBFeatureExtraction OTBFuzzy)
