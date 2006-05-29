PROJECT(Learning)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(GenerateTrainingImageExample GenerateTrainingImageExample.cxx )
TARGET_LINK_LIBRARIES(GenerateTrainingImageExample OTBCommon OTBIO OTBLearning ITKCommon ITKIO)

ADD_EXECUTABLE(SVMPointSetExample SVMPointSetExample.cxx )
TARGET_LINK_LIBRARIES(SVMPointSetExample ITKIO OTBIO OTBCommon
ITKCommon  OTBLearning ${CAI_LIBRARY} ${GDAL_LIBRARY})


ADD_EXECUTABLE(SVMPointSetModelEstimatorExample SVMPointSetModelEstimatorExample.cxx )
TARGET_LINK_LIBRARIES(SVMPointSetModelEstimatorExample ITKIO OTBIO
OTBCommon ITKCommon  OTBLearning ${CAI_LIBRARY} ${GDAL_LIBRARY})

ADD_EXECUTABLE(SVMImageModelEstimatorExample SVMImageModelEstimatorExample.cxx )
TARGET_LINK_LIBRARIES(SVMImageModelEstimatorExample ITKIO OTBIO
OTBCommon ITKCommon  OTBLearning ${CAI_LIBRARY} ${GDAL_LIBRARY})

ADD_EXECUTABLE(SVMPointSetClassificationExample SVMPointSetClassificationExample.cxx )
TARGET_LINK_LIBRARIES(SVMPointSetClassificationExample ITKIO OTBIO
OTBCommon ITKCommon  OTBLearning ${CAI_LIBRARY} ${GDAL_LIBRARY})

ADD_EXECUTABLE(SVMImageClassificationExample SVMImageClassificationExample.cxx )
TARGET_LINK_LIBRARIES(SVMImageClassificationExample ITKIO OTBIO
OTBCommon ITKCommon  OTBLearning ${CAI_LIBRARY} ${GDAL_LIBRARY})

ADD_EXECUTABLE(SVMImageEstimatorClassificationMultiExample SVMImageEstimatorClassificationMultiExample.cxx )
TARGET_LINK_LIBRARIES(SVMImageEstimatorClassificationMultiExample ITKIO OTBIO
OTBCommon ITKCommon  OTBLearning ${CAI_LIBRARY} ${GDAL_LIBRARY})

