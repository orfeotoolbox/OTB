PROJECT(LearningExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(GenerateTrainingImageExample GenerateTrainingImageExample.cxx )
TARGET_LINK_LIBRARIES(GenerateTrainingImageExample OTBCommon OTBIO OTBLearning ITKCommon ITKIO)

ADD_EXECUTABLE(SVMPointSetExample SVMPointSetExample.cxx )
TARGET_LINK_LIBRARIES(SVMPointSetExample ITKIO OTBIO OTBCommon
ITKCommon  OTBLearning  gdal)


ADD_EXECUTABLE(SVMPointSetModelEstimatorExample SVMPointSetModelEstimatorExample.cxx )
TARGET_LINK_LIBRARIES(SVMPointSetModelEstimatorExample ITKIO OTBIO
OTBCommon ITKCommon  OTBLearning  gdal)

ADD_EXECUTABLE(SVMImageModelEstimatorExample SVMImageModelEstimatorExample.cxx )
TARGET_LINK_LIBRARIES(SVMImageModelEstimatorExample ITKIO OTBIO
OTBCommon ITKCommon  OTBLearning  gdal)

ADD_EXECUTABLE(SVMPointSetClassificationExample SVMPointSetClassificationExample.cxx )
TARGET_LINK_LIBRARIES(SVMPointSetClassificationExample ITKIO OTBIO
OTBCommon ITKCommon  OTBLearning  gdal)

ADD_EXECUTABLE(SVMImageClassificationExample SVMImageClassificationExample.cxx )
TARGET_LINK_LIBRARIES(SVMImageClassificationExample ITKIO OTBIO
OTBCommon ITKCommon  OTBLearning  gdal)

ADD_EXECUTABLE(SVMImageEstimatorClassificationMultiExample SVMImageEstimatorClassificationMultiExample.cxx )
TARGET_LINK_LIBRARIES(SVMImageEstimatorClassificationMultiExample ITKIO OTBIO
OTBCommon ITKCommon  OTBLearning  gdal)

ADD_EXECUTABLE(SOMExample SOMExample.cxx )
TARGET_LINK_LIBRARIES(SOMExample ITKIO OTBIO OTBCommon ITKCommon  OTBLearning  gdal)


IF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Learning)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbLearningExamplesTests)

SET(TOL 0.0)


# ------- SVMImageClassificationExampleTest----------

ADD_TEST(SVMImageClassificationExampleTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/ROI_QB_MUL_1_SVN_CLASS.png
	${TEMP}/ROI_QB_MUL_1_SVN_CLASS.png
	SVMImageClassificationExampleTest
	${INPUTDATA}/ROI_QB_MUL_1.png
	${TEMP}/ROI_QB_MUL_1_SVN_CLASS.png
    ${OTB_SOURCE_DIR}/Examples/Data/svm_image_model.svm
)

# ------- SVMImageEstimatorClassificationMultiExampleTest----------

ADD_TEST(SVMImageEstimatorClassificationMultiExampleTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
	${TEMP}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
	SVMImageEstimatorClassificationMultiExampleTest
	${INPUTDATA}/ROI_QB_MUL_1.png
	${INPUTDATA}/ROI_mask_multi.png
	${TEMP}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
)

# ------- GenerateTrainingImageExampleTest----------

ADD_TEST(GenerateTrainingImageExampleTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/ROI_QB_MUL_4_training.png
	${TEMP}/ROI_QB_MUL_4_training.png
	GenerateTrainingImageExampleTest
	${INPUTDATA}/ROI_QB_MUL_4.tif
	${INPUTDATA}/LearningROIs.txt
	${TEMP}/ROI_QB_MUL_4_training.png
)

# ------- SVMImageModelEstimatorExampleTest----------

ADD_TEST(SVMImageModelEstimatorExampleTest ${EXE_TESTS}
	--compare-ascii ${TOL}
	${BASELINE}/svm_image_model.svn
	${TEMP}/svm_image_model.svn
	SVMImageModelEstimatorExampleTest
	${INPUTDATA}/ROI_QB_MUL_1.png
	${INPUTDATA}/ROI_mask.png
	${TEMP}/svm_image_model.svn
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbLearningExamplesTests otbLearningExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbLearningExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )
