
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(LEARNING_TESTS ${CXX_TEST_PATH}/otbLearningTests)

#EPSILON est different de 0.0 pour les tests multiplateformes avec différentes options de compilation.
SET(EPSILON 0.0000000001)


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

#ADD_TEST(leTuSVMMembershipFunctionNew ${LEARNING_TESTS}  
#        otbSVMMembershipFunctionNew)

#ADD_TEST(leTuSVMMembershipFunctionLoadModel ${LEARNING_TESTS}  
#        otbSVMMembershipFunctionLoadModel
#	${INPUTDATA}/svm_model)

# -------            otb::SVMImageModelEstimator   --------------------	

ADD_TEST(leTuSVMImageModelEstimatorNew ${LEARNING_TESTS}  
        otbSVMImageModelEstimatorNew)

ADD_TEST(leTuSVMImageModelEstimatorTrain ${LEARNING_TESTS}  
        otbSVMImageModelEstimatorTrain
	${INPUTDATA}/ROI_QB_MUL_4.tif
	${INPUTDATA}/ROI_QB_MUL_4_train.c1.hdr
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
	

# -------            otb::SOMMap   -----------------------------	

ADD_TEST(leTuSOMMapNew ${LEARNING_TESTS}  
        otbSOMMapNew)

ADD_TEST(leTvSOMMap ${LEARNING_TESTS}  
        otbSOMMap)

# -------            otb::SOM   -----------------------------	

ADD_TEST(leTuSOMNew ${LEARNING_TESTS}  
        otbSOMNew)

ADD_TEST(leTvSOM ${LEARNING_TESTS}  
        --compare-image ${EPSILON}
	${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
	${TEMP}/leSOMPoupeesSubOutputMap1.hdr
	otbSOM
	${INPUTDATA}/poupees_sub.png
	${TEMP}/leSOMPoupeesSubOutputMap1.hdr
	32 32 10 10 5 1.0 0.1 0)

# -------            otb::SOMActivationBuilder   -----------------------------	

ADD_TEST(leTuSOMActivationBuilderNew ${LEARNING_TESTS}  
        otbSOMActivationBuilderNew)

ADD_TEST(leTvSOMActivationBuilder ${LEARNING_TESTS}  
        --compare-image ${TOL}
	${BASELINE}/leSOMPoupeesSubActivationMap1.hdr
	${TEMP}/leSOMPoupeesSubActivationMap1.hdr
	otbSOMActivationBuilder
	${INPUTDATA}/poupees_sub.png
	${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
	${TEMP}/leSOMPoupeesSubActivationMap1.hdr
	32 32 10 10 5 1.0 0.1 128)

# -----------------------------------------------------------------------
			        


# A enrichir
SET(BasicLearning_SRCS
otbSVMModelNew.cxx
otbSVMModelAllocateProblem.cxx
otbSVMModelLoad.cxx
otbSVMModelLoadSave.cxx
#otbSVMMembershipFunctionNew.cxx
#otbSVMMembershipFunctionLoadModel.cxx
otbSVMImageModelEstimatorNew.cxx
otbSVMImageModelEstimatorTrain.cxx
otbSVMPointSetModelEstimatorNew.cxx
otbSVMPointSetModelEstimatorTrain.cxx
otbSVMClassifierNew.cxx
otbSVMClassifierImage.cxx
otbSVMClassifierPointSet.cxx
otbSOMMapNew.cxx
otbSOMMap.cxx
otbSOMNew.cxx
otbSOM.cxx
otbSOMActivationBuilderNew.cxx
otbSOMActivationBuilder.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbLearningTests otbLearningTests.cxx ${BasicLearning_SRCS})
TARGET_LINK_LIBRARIES(otbLearningTests gdal otbsvm OTBLearning OTBCommon OTBIO ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




