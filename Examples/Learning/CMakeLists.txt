PROJECT(Learning)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(GenerateTrainingImageExample GenerateTrainingImageExample.cxx )
TARGET_LINK_LIBRARIES(GenerateTrainingImageExample OTBCommon OTBIO OTBLearning ITKCommon ITKIO)

ADD_EXECUTABLE(SVMPointSetExample SVMPointSetExample.cxx )
TARGET_LINK_LIBRARIES(SVMPointSetExample ITKIO OTBIO OTBCommon ITKCommon  OTBLearning ${CAI_LIBRARY} ${GDAL_LIBRARY})