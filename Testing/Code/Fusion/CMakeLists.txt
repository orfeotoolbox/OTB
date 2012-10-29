
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(FUSION_TESTS1 ${CXX_TEST_PATH}/otbFusionTests1)

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
    --compare-image ${NOTOL}  ${BASELINE}/fuTvBayesianFusion.tif
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
    --compare-image ${EPSILON_8}  ${BASELINE}/fuTvRcsPanSharpeningFusion.tif
                           ${TEMP}/fuTvRcsPanSharpeningFusion.tif
        otbSimpleRcsPanSharpeningFusionImageFilter
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
        ${TEMP}/fuTvRcsPanSharpeningFusion.tif
)

# -------    otb::LmvmPanSharpeningFusionImageFilter   -------------------
ADD_TEST(fuTuLmvmPanSharpeningFusionImageFilterNew ${FUSION_TESTS1}
        otbLmvmPanSharpeningFusionImageFilterNew
)

ADD_TEST(fuTvLmvmPanSharpeningFusionImageFilter ${FUSION_TESTS1}
    --compare-image ${EPSILON_8}  ${BASELINE}/fuTvLmvmPanSharpeningFusion.tif
                           ${TEMP}/fuTvLmvmPanSharpeningFusion.tif
        otbLmvmPanSharpeningFusionImageFilter
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
        ${TEMP}/fuTvLmvmPanSharpeningFusion.tif
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFusion2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# A enrichir
SET(Fusion_SRCS1
otbFusionTests1.cxx
otbFusionImageBaseNew.cxx
otbBayesianFusionFilterNew.cxx
otbBayesianFusionFilter.cxx
otbSimpleRcsPanSharpeningFusionImageFilterNew.cxx
otbSimpleRcsPanSharpeningFusionImageFilter.cxx
otbLmvmPanSharpeningFusionImageFilterNew.cxx
otbLmvmPanSharpeningFusionImageFilter.cxx
)

OTB_ADD_EXECUTABLE(otbFusionTests1 "${Fusion_SRCS1}" "OTBFusion;OTBIO;OTBTesting")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

