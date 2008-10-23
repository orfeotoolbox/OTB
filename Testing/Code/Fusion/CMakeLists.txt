
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance de base
SET(TOL 0.0)

#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
SET(EPS 0.001)

#Tolerance sur diff pixel image
#EPSILON est different de 0.0 pour les tests multiplateformes avec différentes options de compilation.
SET(EPSILON 0.00000001)

SET(FUSION_TESTS1 ${CXX_TEST_PATH}/otbFusionTests1)
SET(FUSION_TESTS2 ${CXX_TEST_PATH}/otbFusionTests2)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFusion1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

# -------            otb::ImageFusionBase   ------------------------------
ADD_TEST(fuTuFusionImageBaseNew ${FUSION_TESTS1}  
        otbFusionImageBaseNew
)
# -------            otb::BayesianFusionFilter   ------------------------------
ADD_TEST(fuTuBayesianFusionFilterNew ${FUSION_TESTS1}  
        otbBayesianFusionFilterNew
)

ADD_TEST(fuTvBayesianFusionFilter ${FUSION_TESTS1} 
    --compare-image ${TOL}  ${BASELINE}/fuTvBayesianFusion.tif
                           ${TEMP}/fuTvBayesianFusion.tif
        otbBayesianFusionFilter
	${INPUTDATA}/multiSpect.tif 
	${INPUTDATA}/multiSpectInterp.tif 
	${INPUTDATA}/panchro.tif 
	${TEMP}/fuTvBayesianFusion.tif
)


# -------    otb::SimpleRcsPanSharpeningFusionImageFilter   -------------------
ADD_TEST(fuTuSimpleRcsPanSharpeningFusionImageFilterNew ${FUSION_TESTS1}  
        otbSimpleRcsPanSharpeningFusionImageFilterNew
)

ADD_TEST(fuTvSimpleRcsPanSharpeningFusionImageFilter ${FUSION_TESTS1} 
    --compare-image ${EPSILON}  ${BASELINE}/fuTvRcsPanSharpeningFusion.tif
                           ${TEMP}/fuTvRcsPanSharpeningFusion.tif
        otbSimpleRcsPanSharpeningFusionImageFilter
	${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
	${INPUTDATA}/QB_Toulouse_Ortho_XS.tif 
	${TEMP}/fuTvRcsPanSharpeningFusion.tif
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFusion2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
        
# A enrichir
SET(Fusion_SRCS1
otbFusionImageBaseNew.cxx
otbBayesianFusionFilterNew.cxx
otbBayesianFusionFilter.cxx
otbSimpleRcsPanSharpeningFusionImageFilterNew.cxx
otbSimpleRcsPanSharpeningFusionImageFilter.cxx
)
SET(Fusion_SRCS2
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbFusionTests1 otbFusionTests1.cxx ${Fusion_SRCS1})
TARGET_LINK_LIBRARIES(otbFusionTests1  OTBFusion OTBIO)
ADD_EXECUTABLE(otbFusionTests2 otbFusionTests2.cxx ${Fusion_SRCS2})
TARGET_LINK_LIBRARIES(otbFusionTests2  OTBFusion OTBIO)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

