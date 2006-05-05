
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(LEARNING_TESTS ${CXX_TEST_PATH}/otbLearningTests)



# -------            otb::SVMModel   ------------------------------

ADD_TEST(leTuSVMModelNew ${LEARNING_TESTS}  
        otbSVMModelNew)

ADD_TEST(leTuSVMModelAllocateProblem ${LEARNING_TESTS}  
        otbSVMModelAllocateProblem)	

ADD_TEST(leTuSVMModelLoad ${LEARNING_TESTS}  
        otbSVMModelLoad
	${INPUTDATA}/svm_model)

ADD_TEST(leTuSVMModelLoadSave ${LEARNING_TESTS}  
        otbSVMModelLoadSave
	${INPUTDATA}/svm_model
	${TEMP}/svmmodel_test)		

# -------            otb::SVMembershipFunction   --------------------

ADD_TEST(leTuSVMMembershipFunctionNew ${LEARNING_TESTS}  
        otbSVMMembershipFunctionNew)

ADD_TEST(leTuSVMMembershipFunctionLoadModel ${LEARNING_TESTS}  
        otbSVMMembershipFunctionLoadModel
	${INPUTDATA}/svm_model)

# -------            otb::SVMImageModelEstimator   --------------------	

ADD_TEST(leTuSVMImageModelEstimatorNew ${LEARNING_TESTS}  
        otbSVMImageModelEstimatorNew)

ADD_TEST(leTuSVMImageModelEstimatorTrain ${LEARNING_TESTS}  
        otbSVMImageModelEstimatorTrain
	${INPUTDATA}/ROI_QB_MUL_4.tif
	${INPUTDATA}/ROI_QB_MUL_4_train.hd
	${TEMP}/svm_model_image)

# -------            otb::SVMPointSetModelEstimator   --------------------		

ADD_TEST(leTuSVMPointSetModelEstimatorNew ${LEARNING_TESTS}  
        otbSVMPointSetModelEstimatorNew)

ADD_TEST(leTuSVMPointSetModelEstimatorTrain ${LEARNING_TESTS}  
        otbSVMPointSetModelEstimatorTrain
	${TEMP}/svm_model_pointset)
	
	
# -------            otb::SVMClassifier   -----------------------------	

ADD_TEST(leTuSVMClassifierNew ${LEARNING_TESTS}  
        otbSVMClassifierNew)

ADD_TEST(leTuSVMClassifierImage ${LEARNING_TESTS}  
        otbSVMClassifierImage
	${INPUTDATA}/ROI_QB_MUL_4.tif
	${INPUTDATA}/svm_model_image
	${TEMP}/svm_classified_image.png)

ADD_TEST(leTuSVMClassifierPointSet ${LEARNING_TESTS}  
        otbSVMClassifierPointSet
	${INPUTDATA}/svm_model_pointset)
	

# -----------------------------------------------------------------------
			        


# A enrichir
SET(BasicLearning_SRCS
otbSVMModelNew.cxx
otbSVMModelAllocateProblem.cxx
otbSVMModelLoad.cxx
otbSVMModelLoadSave.cxx
otbSVMMembershipFunctionNew.cxx
otbSVMMembershipFunctionLoadModel.cxx
otbSVMImageModelEstimatorNew.cxx
otbSVMImageModelEstimatorTrain.cxx
otbSVMPointSetModelEstimatorNew.cxx
otbSVMPointSetModelEstimatorTrain.cxx
otbSVMClassifierNew.cxx
otbSVMClassifierImage.cxx
otbSVMClassifierPointSet.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbLearningTests otbLearningTests.cxx ${BasicLearning_SRCS})
TARGET_LINK_LIBRARIES(otbLearningTests ${CAI_LIBRARY} ${GDAL_LIBRARY} OTBLearning OTBCommon OTBIO ${CAI_LIBRARY} ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




