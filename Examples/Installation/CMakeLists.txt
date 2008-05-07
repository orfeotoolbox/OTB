PROJECT(Classif)

FIND_PACKAGE(OTB)
IF(OTB_FOUND)
  INCLUDE(${OTB_USE_FILE})
ELSE(OTB_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build HelloWorld application without OTB.  Please set OTB_DIR.")
ENDIF(OTB_FOUND)

ADD_EXECUTABLE(SVMImageEstimatorClassificationMultiExample SVMImageEstimatorClassificationMultiExample.cxx )

TARGET_LINK_LIBRARIES(SVMImageEstimatorClassificationMultiExample ITKIO OTBIO OTBCommon ITKCommon  OTBLearning)
