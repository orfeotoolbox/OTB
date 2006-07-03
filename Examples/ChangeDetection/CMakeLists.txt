PROJECT(FeatureExtraction)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ChangeDetectionFrameworkExample ChangeDetectionFrameworkExample.cxx )
TARGET_LINK_LIBRARIES(ChangeDetectionFrameworkExample OTBCommon gdal OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(DiffChDet DiffChDet.cxx)
TARGET_LINK_LIBRARIES(DiffChDet OTBIO OTBCommon  gdal ITKCommon ITKIO)

ADD_EXECUTABLE(RatioChDet RatioChDet.cxx)
TARGET_LINK_LIBRARIES(RatioChDet OTBIO OTBCommon  gdal ITKCommon ITKIO)

ADD_EXECUTABLE(CorrelChDet CorrelChDet.cxx)
TARGET_LINK_LIBRARIES(CorrelChDet OTBIO OTBCommon  gdal ITKCommon ITKIO)

ADD_EXECUTABLE(LHMIChDet LHMIChDet.cxx)
TARGET_LINK_LIBRARIES(LHMIChDet OTBIO OTBCommon gdal ITKCommon ITKIO ITKStatistics)

ADD_EXECUTABLE(JHMIChDet JHMIChDet.cxx)
TARGET_LINK_LIBRARIES(JHMIChDet OTBIO OTBCommon gdal ITKCommon ITKIO ITKStatistics)

