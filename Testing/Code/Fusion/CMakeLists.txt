
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(FUSION_TESTS1 ${CXX_TEST_PATH}/otbFusionTests1)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFusion1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::ImageFusionBase   ------------------------------
add_test(fuTuFusionImageBaseNew ${FUSION_TESTS1}
        otbFusionImageBaseNew
)
# -------            otb::BayesianFusionFilter   ------------------------------
add_test(fuTuBayesianFusionFilterNew ${FUSION_TESTS1}
        otbBayesianFusionFilterNew
)

add_test(fuTvBayesianFusionFilter ${FUSION_TESTS1}
    --compare-image ${NOTOL}  ${BASELINE}/fuTvBayesianFusion.tif
                           ${TEMP}/fuTvBayesianFusion.tif
        otbBayesianFusionFilter
        ${INPUTDATA}/multiSpect.tif
        ${INPUTDATA}/multiSpectInterp.tif
        ${INPUTDATA}/panchro.tif
        ${TEMP}/fuTvBayesianFusion.tif
)


# -------    otb::SimpleRcsPanSharpeningFusionImageFilter   -------------------
add_test(fuTuSimpleRcsPanSharpeningFusionImageFilterNew ${FUSION_TESTS1}
        otbSimpleRcsPanSharpeningFusionImageFilterNew
)

add_test(fuTvSimpleRcsPanSharpeningFusionImageFilter ${FUSION_TESTS1}
    --compare-image ${EPSILON_8}  ${BASELINE}/fuTvRcsPanSharpeningFusion.tif
                           ${TEMP}/fuTvRcsPanSharpeningFusion.tif
        otbSimpleRcsPanSharpeningFusionImageFilter
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
        ${TEMP}/fuTvRcsPanSharpeningFusion.tif
)

# -------    otb::LmvmPanSharpeningFusionImageFilter   -------------------
add_test(fuTuLmvmPanSharpeningFusionImageFilterNew ${FUSION_TESTS1}
        otbLmvmPanSharpeningFusionImageFilterNew
)

add_test(fuTvLmvmPanSharpeningFusionImageFilter ${FUSION_TESTS1}
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
set(Fusion_SRCS1
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

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

