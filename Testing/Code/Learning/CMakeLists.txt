
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

#EPSILON est different de 0.0 pour les tests multiplateformes avec différentes options de compilation.
SET(EPSILON 0.0000000001)
SET(EPSILON_10e3 0.001)
SET(EPSILON_10e4 0.0001)


SET(LEARNING_TESTS1 ${CXX_TEST_PATH}/otbLearningTests1)
SET(LEARNING_TESTS2 ${CXX_TEST_PATH}/otbLearningTests2)
SET(LEARNING_TESTS3 ${CXX_TEST_PATH}/otbLearningTests3)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbLearningTESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::SVMModel   ------------------------------

ADD_TEST(leTuSVMModelNew ${LEARNING_TESTS1}  
        otbSVMModelNew)

ADD_TEST(leTuSVMModelAllocateProblem ${LEARNING_TESTS1}  
        otbSVMModelAllocateProblem)	

ADD_TEST(leTuSVMModelLoad ${LEARNING_TESTS1}  
        otbSVMModelLoad
	${INPUTDATA}/svm_model)

ADD_TEST(leTvSVMModelAccessor ${LEARNING_TESTS1}
--compare-ascii ${TOL}  ${BASELINE_FILES}/leSVMModelAccessor.txt
                        ${TEMP}/leSVMModelAccessor.txt
        otbSVMModelAccessor
	${INPUTDATA}/svm_model
	${TEMP}/leSVMModelAccessor.txt)		


ADD_TEST(leTvSVMModelLoadSave ${LEARNING_TESTS1}
--compare-ascii ${TOL}  ${INPUTDATA}/svm_model
                        ${TEMP}/svmmodel_test
        otbSVMModelLoadSave
	${INPUTDATA}/svm_model
	${TEMP}/svmmodel_test)		

ADD_TEST(leTvSVMModelWithGenericKeywordKernelLoadSave ${LEARNING_TESTS1}  
--compare-ascii ${TOL}  ${BASELINE_FILES}/leTvsvmmodel_image_generic_version_test
                        ${TEMP}/leTvsvmmodel_image_generic_version_test
        otbSVMModelGenericKernelLoadSave
	${INPUTDATA}/svm_model_image_generic_version
	${TEMP}/leTvsvmmodel_image_generic_version_test)		

ADD_TEST(leTvSVMModelKernelTypeGenericLoadSave ${LEARNING_TESTS1}  
--compare-ascii ${TOL}  ${INPUTDATA}/svm_model_image_generic_kernel
                        ${TEMP}/leTvsvmmodel_image_generic_kernel_test
        otbSVMModelGenericKernelLoadSave
	${INPUTDATA}/svm_model_image_generic_kernel
	${TEMP}/leTvsvmmodel_image_generic_kernel_test)		

# -------            otb::SVMembershipFunction   --------------------

#ADD_TEST(leTuSVMMembershipFunctionNew ${LEARNING_TESTS1}  
#        otbSVMMembershipFunctionNew)

#ADD_TEST(leTuSVMMembershipFunctionLoadModel ${LEARNING_TESTS1}  
#        otbSVMMembershipFunctionLoadModel
#	${INPUTDATA}/svm_model)

# -------            otb::SVMImageModelEstimator   --------------------	

ADD_TEST(leTuSVMImageModelEstimatorNew ${LEARNING_TESTS1}  
        otbSVMImageModelEstimatorNew)

ADD_TEST(leTvSVMImageModelEstimatorTrain ${LEARNING_TESTS1}
--compare-ascii ${EPSILON_10e3}  ${BASELINE_FILES}/leTvsvm_model_image 
                        ${TEMP}/leTvsvm_model_image 
        otbSVMImageModelEstimatorTrain
	${INPUTDATA}/ROI_QB_MUL_4.tif
	${INPUTDATA}/ROI_QB_MUL_4_train.c1.hdr
	${TEMP}/leTvsvm_model_image)

ADD_TEST(leTvSVMImageModelEstimatorModelAccessor ${LEARNING_TESTS1}
--compare-ascii ${EPSILON_10e3}  ${BASELINE_FILES}/leSVMModelEstimatorModelAccessor.txt
                        ${TEMP}/leSVMModelEstimatorModelAccessor.txt
        otbSVMImageModelEstimatorModelAccessor
	${INPUTDATA}/ROI_QB_MUL_4.tif
	${INPUTDATA}/ROI_QB_MUL_4_train.c1.hdr
	${TEMP}/leSVMModelEstimatorModelAccessor.txt)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbLearningTESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::SVMPointSetModelEstimator   --------------------		

ADD_TEST(leTuSVMPointSetModelEstimatorNew ${LEARNING_TESTS2}  
        otbSVMPointSetModelEstimatorNew)

ADD_TEST(leTuSVMPointSetModelEstimatorTrain ${LEARNING_TESTS2}  
--compare-ascii ${EPSILON_10e3}  ${BASELINE_FILES}/leTvsvm_model_pointset 
                        ${TEMP}/leTvsvm_model_pointset 
        otbSVMPointSetModelEstimatorTrain
	${TEMP}/leTvsvm_model_pointset)
	
	
# -------            otb::SVMClassifier   -----------------------------	

ADD_TEST(leTuSVMClassifierNew ${LEARNING_TESTS2}  
        otbSVMClassifierNew)

ADD_TEST(leTvSVMClassifierImage ${LEARNING_TESTS2}  
        otbSVMClassifierImage
	${INPUTDATA}/ROI_QB_MUL_4.tif
	${INPUTDATA}/svm_model_image
	${TEMP}/svm_classified_image.png)

ADD_TEST(leTvSVMClassifierImageWithGenericKernalLinear ${LEARNING_TESTS2}  
--compare-image ${TOL}  ${TEMP}/svm_classified_image.png
                        ${TEMP}/svm_classified_image_with_generic_kernel_linear.png
        otbSVMClassifierImage
	${INPUTDATA}/ROI_QB_MUL_4.tif
	${INPUTDATA}/svm_model_image_generic_linear
	${TEMP}/svm_classified_image_with_generic_kernel_linear.png)



ADD_TEST(leTvSVMClassifierPointSet ${LEARNING_TESTS2}  
        otbSVMClassifierPointSet
	${INPUTDATA}/svm_model_pointset)
	

# -------            otb::SOMMap   -----------------------------	

ADD_TEST(leTuSOMMapNew ${LEARNING_TESTS2}  
        otbSOMMapNew)

ADD_TEST(leTvSOMMap ${LEARNING_TESTS2}  
        otbSOMMap)

# -------            otb::SOM   -----------------------------	

ADD_TEST(leTuSOMNew ${LEARNING_TESTS2}  
        otbSOMNew)

ADD_TEST(leTvSOM ${LEARNING_TESTS2}  
        --compare-image ${EPSILON}
	${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
	${TEMP}/leSOMPoupeesSubOutputMap1.hdr
	otbSOM
	${INPUTDATA}/poupees_sub.png
	${TEMP}/leSOMPoupeesSubOutputMap1.hdr
	32 32 10 10 5 1.0 0.1 0)

# -------            otb::PeriodicSOM   -----------------------------	

ADD_TEST(leTuPeriodicSOMNew ${LEARNING_TESTS2}  
        otbPeriodicSOMNew)

ADD_TEST(leTvPeriodicSOM ${LEARNING_TESTS2}  
        --compare-image ${EPSILON}
	${BASELINE}/lePeriodicSOMPoupeesSubOutputMap1.hdr
	${TEMP}/lePeriodicSOMPoupeesSubOutputMap1.hdr
	otbSOM
	${INPUTDATA}/poupees_sub.png
	${TEMP}/lePeriodicSOMPoupeesSubOutputMap1.hdr
	32 32 10 10 5 1.0 0.1 0)

# -------            otb::SOMActivationBuilder   -----------------------------	

ADD_TEST(leTuSOMActivationBuilderNew ${LEARNING_TESTS2}  
        otbSOMActivationBuilderNew)

ADD_TEST(leTvSOMActivationBuilder ${LEARNING_TESTS2}  
        --compare-image ${TOL}
	${BASELINE}/leSOMPoupeesSubActivationMap1.hdr
	${TEMP}/leSOMPoupeesSubActivationMap1.hdr
	otbSOMActivationBuilder
	${INPUTDATA}/poupees_sub.png
	${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
	${TEMP}/leSOMPoupeesSubActivationMap1.hdr
	32 32 10 10 5 1.0 0.1 128)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbLearningTESTS3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::SOMClassifier   -----------------------------	

ADD_TEST(leTuSOMClassifierNew ${LEARNING_TESTS3}  
        otbSOMClassifierNew)

ADD_TEST(leTvSOMClassifier ${LEARNING_TESTS3}  
        --compare-image ${TOL}
	${BASELINE}/leSOMPoupeesClassified.hdr
	${TEMP}/leSOMPoupeesClassified.hdr
	otbSOMClassifier
	${INPUTDATA}/poupees_sub.png
	${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
	${TEMP}/leSOMPoupeesClassified.hdr
	)

# -------            otb::SOMImageClassificationFilter   -----------------------------	

ADD_TEST(leTuSOMImageClassificationFilterNew ${LEARNING_TESTS3}  
        otbSOMImageClassificationFilterNew)

ADD_TEST(leTvSOMImageClassificationFilter ${LEARNING_TESTS3}  
        --compare-image ${TOL}
	${BASELINE}/leSOMPoupeesClassified.hdr
	${TEMP}/leSOMImageClassificationFilterOuptut.hdr
	otbSOMImageClassificationFilter
	${INPUTDATA}/poupees_sub.png
	${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
	${TEMP}/leSOMImageClassificationFilterOuptut.hdr
	)


# -------            otb::KMeansImageClassificationFilter   -----------------------------	

ADD_TEST(leTuKMeansImageClassificationFilterNew ${LEARNING_TESTS3}  
        otbKMeansImageClassificationFilterNew)

ADD_TEST(leTvKMeansImageClassificationFilter ${LEARNING_TESTS3}  
        --compare-image ${TOL}
	${BASELINE}/leKMeansImageClassificationFilterOuptut.hdr
	${TEMP}/leKMeansImageClassificationFilterOuptut.hdr
	otbKMeansImageClassificationFilter
	${INPUTDATA}/poupees_sub.png
	${TEMP}/leKMeansImageClassificationFilterOuptut.hdr
	2
	0 0 0 0
	255 255 255 255
	)

# -------            otb::SEMClassifier   -----------------------------	

ADD_TEST(leTuSEMClassifierNew ${LEARNING_TESTS3}  
        otbSEMClassifierNew)

# -----------------------------------------------------------------------

# ------- otb::SVMImageClassificationFilter ---------------------------

ADD_TEST(leTuSVMImageClassificationFilterNew ${LEARNING_TESTS3}
	otbSVMImageClassificationFilterNew)

ADD_TEST(leTvSVMImageClassificationFilter ${LEARNING_TESTS3}
	--compare-image ${TOL}
	${BASELINE}/leSVMImageClassificationFilterOutput.tif
	${TEMP}/leSVMImageClassificationFilterOutput.tif
	otbSVMImageClassificationFilter
	${INPUTDATA}/ROI_QB_MUL_4.tif
	${INPUTDATA}/svm_model_image
	${TEMP}/leSVMImageClassificationFilterOutput.tif)
			        
ADD_TEST(leTuSVMModelGenericKernelsTest ${LEARNING_TESTS3} 
	otbSVMModelGenericKernelsTest)


ADD_TEST(leTvSVMModelCopy ${LEARNING_TESTS3}
--compare-ascii ${TOL}  ${INPUTDATA}/svm_model
                        ${TEMP}/svmcopymodel_test
        otbSVMModelCopyTest
	${INPUTDATA}/svm_model
	${TEMP}/svmcopymodel_test)

ADD_TEST(leTvSVMModelCopyGenericKernel ${LEARNING_TESTS3}
--compare-ascii ${TOL}  ${INPUTDATA}/svm_model_generic
                        ${TEMP}/svmcopygeneric_test
        otbSVMModelCopyGenericKernelTest
	${INPUTDATA}/svm_model_generic
	${TEMP}/svmcopygeneric_test)

ADD_TEST(leTvSVMModelCopyComposedKernel ${LEARNING_TESTS3}
--compare-ascii ${TOL}  ${INPUTDATA}/svm_model_composed
                        ${TEMP}/svmcopycomposed_test
        otbSVMModelCopyComposedKernelTest
	${INPUTDATA}/svm_model_composed
	${TEMP}/svmcopycomposed_test)



# A enrichir
SET(BasicLearning_SRCS1
otbSVMModelNew.cxx
otbSVMModelAllocateProblem.cxx
otbSVMModelLoad.cxx
otbSVMModelAccessor.cxx
otbSVMModelLoadSave.cxx
otbSVMModelGenericKernelLoadSave.cxx
#otbSVMMembershipFunctionNew.cxx
#otbSVMMembershipFunctionLoadModel.cxx
otbSVMImageModelEstimatorNew.cxx
otbSVMImageModelEstimatorTrain.cxx
otbSVMImageModelEstimatorModelAccessor.cxx
)
SET(BasicLearning_SRCS2
otbSVMPointSetModelEstimatorNew.cxx
otbSVMPointSetModelEstimatorTrain.cxx
otbSVMClassifierNew.cxx
otbSVMClassifierImage.cxx
otbSVMClassifierPointSet.cxx
otbSOMMapNew.cxx
otbSOMMap.cxx
otbSOMNew.cxx
otbSOM.cxx
otbPeriodicSOMNew.cxx
otbPeriodicSOM.cxx
otbSOMActivationBuilderNew.cxx
otbSOMActivationBuilder.cxx
)
SET(BasicLearning_SRCS3
otbSOMClassifierNew.cxx
otbSOMClassifier.cxx
otbSOMImageClassificationFilterNew.cxx
otbSOMImageClassificationFilter.cxx
otbKMeansImageClassificationFilterNew.cxx
otbKMeansImageClassificationFilter.cxx
otbSEMClassifierNew.cxx
otbSVMImageClassificationFilterNew.cxx
otbSVMImageClassificationFilter.cxx
otbSVMModelGenericKernelsTest.cxx
otbSVMModelCopyTest.cxx
otbSVMModelCopyGenericKernelTest.cxx
otbSVMModelCopyComposedKernelTest.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbLearningTests1 otbLearningTests1.cxx ${BasicLearning_SRCS1})
TARGET_LINK_LIBRARIES(otbLearningTests1 gdal otbsvm OTBLearning OTBCommon OTBIO ITKIO ITKCommon)
ADD_EXECUTABLE(otbLearningTests2 otbLearningTests2.cxx ${BasicLearning_SRCS2})
TARGET_LINK_LIBRARIES(otbLearningTests2 gdal otbsvm OTBLearning OTBCommon OTBIO ITKIO ITKCommon)
ADD_EXECUTABLE(otbLearningTests3 otbLearningTests3.cxx ${BasicLearning_SRCS3})
TARGET_LINK_LIBRARIES(otbLearningTests3 gdal otbsvm OTBLearning OTBCommon OTBIO ITKIO ITKCommon)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )




