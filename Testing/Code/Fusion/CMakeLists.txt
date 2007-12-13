
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance de base
SET(TOL 0.0)

#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
SET(EPS 0.001)

#Tolerance sur diff pixel image
#EPSILON est different de 0.0 pour les tests multiplateformes avec différentes options de compilation.
SET(EPSILON 0.00000001)

SET(FUSION_TESTS ${CXX_TEST_PATH}/otbFusionTests)


# -------            otb::ImageFusionBase   ------------------------------
ADD_TEST(fuTuFusionImageBaseNew ${FUSION_TESTS}  
        otbFusionImageBaseNew
)
# -------            otb::BayesianFusionFilter   ------------------------------
ADD_TEST(fuTuBayesianFusionFilterNew ${FUSION_TESTS}  
        otbBayesianFusionFilterNew
)

ADD_TEST(fuTvBayesianFusionFilter ${FUSION_TESTS} 
    --compare-image ${TOL}  ${BASELINE}/fuTvBayesianFusion.tif
                           ${TEMP}/fuTvBayesianFusion.tif
        otbBayesianFusionFilter
	${INPUTDATA}/multiSpect.tif 
	${INPUTDATA}/multiSpectInterp.tif 
	${INPUTDATA}/panchro.tif 
	${TEMP}/fuTvBayesianFusion.tif
)


# -------    otb::SimpleRcsPanSharpeningFusionImageFilter   -------------------
ADD_TEST(fuTuSimpleRcsPanSharpeningFusionImageFilterNew ${FUSION_TESTS}  
        otbSimpleRcsPanSharpeningFusionImageFilterNew
)

ADD_TEST(fuTvSimpleRcsPanSharpeningFusionImageFilter ${FUSION_TESTS} 
    --compare-image ${EPSILON}  ${BASELINE}/fuTvRcsPanSharpeningFusion.tif
                           ${TEMP}/fuTvRcsPanSharpeningFusion.tif
        otbSimpleRcsPanSharpeningFusionImageFilter
	${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
	${INPUTDATA}/QB_Toulouse_Ortho_XS.tif 
	${TEMP}/fuTvRcsPanSharpeningFusion.tif
)


        
# A enrichir
SET(Fusion_SRCS
otbFusionImageBaseNew.cxx
otbBayesianFusionFilterNew.cxx
otbBayesianFusionFilter.cxx
otbSimpleRcsPanSharpeningFusionImageFilterNew.cxx
otbSimpleRcsPanSharpeningFusionImageFilter.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbFusionTests otbFusionTests.cxx ${Fusion_SRCS})
TARGET_LINK_LIBRARIES(otbFusionTests  OTBFusion OTBCommon OTBIO gdal ITKIO ITKCommon ITKAlgorithms)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




