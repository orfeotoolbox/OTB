PROJECT(BasicFiltersExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(LeeImageFilter LeeImageFilter.cxx )
TARGET_LINK_LIBRARIES(LeeImageFilter OTBCommon OTBIO)

ADD_EXECUTABLE(FrostImageFilter FrostImageFilter.cxx )
TARGET_LINK_LIBRARIES(FrostImageFilter OTBCommon OTBIO)

ADD_EXECUTABLE(DEMToRainbowExample DEMToRainbowExample.cxx )
TARGET_LINK_LIBRARIES(DEMToRainbowExample OTBCommon OTBIO)

ADD_EXECUTABLE(ScalingFilterExample ScalingFilterExample.cxx )
TARGET_LINK_LIBRARIES(ScalingFilterExample OTBCommon OTBIO)

ADD_EXECUTABLE(PrintableImageFilterExample PrintableImageFilterExample.cxx )
TARGET_LINK_LIBRARIES(PrintableImageFilterExample OTBCommon OTBIO)

ADD_EXECUTABLE(IndexedToRGBExample IndexedToRGBExample.cxx )
TARGET_LINK_LIBRARIES(IndexedToRGBExample OTBCommon OTBIO)

ADD_EXECUTABLE(MeanShiftSegmentationFilterExample MeanShiftSegmentationFilterExample.cxx )
TARGET_LINK_LIBRARIES(MeanShiftSegmentationFilterExample OTBCommon OTBIO OTBBasicFilters OTBSegmentation)

ADD_EXECUTABLE(HillShadingExample HillShadingExample.cxx )
TARGET_LINK_LIBRARIES(HillShadingExample OTBCommon OTBIO OTBBasicFilters)

ADD_EXECUTABLE(BandMathFilterExample BandMathFilterExample.cxx )
TARGET_LINK_LIBRARIES(BandMathFilterExample OTBCommon OTBIO OTBBasicFilters)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/BasicFilters)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbBasicFiltersExamplesTests)

SET(NOTOL 0.0)


# ------- LeeImageFilterTest----------

ADD_TEST(bfTeLeeImageFilterTest ${EXE_TESTS}
        --compare-n-images ${NOTOL} 1
        ${BASELINE}/GomaSmallLeeFiltered.png
        ${TEMP}/GomaSmallLeeFiltered.png
        LeeImageFilterTest
        ${INPUTDATA}/GomaSmall.png
        ${TEMP}/GomaSmallLeeFiltered.png
    3 1
)

# ------- FrostImageFilterTest----------

ADD_TEST(bfTeFrostImageFilterTest ${EXE_TESTS}
        --compare-n-images ${NOTOL} 1
        ${BASELINE}/GomaSmallFrostFiltered.png
        ${TEMP}/GomaSmallFrostFiltered.png
        FrostImageFilterTest
        ${INPUTDATA}/GomaSmall.png
        ${TEMP}/GomaSmallFrostFiltered.png
    5 0.1
)

# ------- DEMToRainbowExampleTest ----------

ADD_TEST(bfTeDEMToRainbowExampleTest ${EXE_TESTS}
        --compare-n-images ${NOTOL} 1
        ${BASELINE}/DEMToRainbowImageGenerator.png
        ${TEMP}/DEMToRainbowImageGenerator.png
        DEMToRainbowExampleTest
        ${TEMP}/DEMToRainbowImageGenerator.png
    6.5 45.5 500 500 0.002 -0.002 ${OTB_DATA_ROOT}/Examples/DEM_srtm
)


# ------- ScalingFilterExampleTest ----------

ADD_TEST(bfTeScalingFilterExampleTest ${EXE_TESTS}
        --compare-n-images ${NOTOL} 2
        ${BASELINE}/QB_Toulouse_Ortho_PAN_rescaled.png
        ${TEMP}/QB_Toulouse_Ortho_PAN_rescaled.png
        ${BASELINE}/QB_Toulouse_Ortho_PAN_casted.png
        ${TEMP}/QB_Toulouse_Ortho_PAN_casted.png
        ScalingFilterExampleTest
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/QB_Toulouse_Ortho_PAN_rescaled.png
        ${TEMP}/QB_Toulouse_Ortho_PAN_casted.png
)

# ------- PrintableImageFilterTest----------

ADD_TEST(bfTePrintableImageFilterExample1Test ${EXE_TESTS}
        --compare-n-images ${NOTOL} 1
        ${BASELINE}/PrintableExampleOutput1.png
        ${TEMP}/PrintableExampleOutput1.png
        PrintableImageFilterExample1Test
        ${INPUTDATA}/IMAGERY_SSECH.tif
        ${TEMP}/PrintableExampleOutput1.png
    1 2 3
)

ADD_TEST(bfTePrintableImageFilteExample2rTest ${EXE_TESTS}
        --compare-n-images ${NOTOL} 1
        ${BASELINE}/PrintableExampleOutput2.png
        ${TEMP}/PrintableExampleOutput2.png
        PrintableImageFilterExample2Test
        ${INPUTDATA}/IMAGERY_SSECH.tif
        ${TEMP}/PrintableExampleOutput2.png
    1 4 2
)

# ------- IndexedToRGBExampleTest----------

ADD_TEST(bfTeIndexedToRGBExampleTest ${EXE_TESTS}
        --compare-n-images ${NOTOL} 2
        ${BASELINE}/buildingExtractionRGB.png
        ${TEMP}/buildingExtractionRGB.png
        ${BASELINE}/buildingExtractionIndexed_scaled.png
        ${TEMP}/buildingExtractionIndexed_scaled.png
        IndexedToRGBExampleTest
        ${INPUTDATA}/buildingExtractionIndexed.tif
        ${TEMP}/buildingExtractionRGB.png
        ${TEMP}/buildingExtractionIndexed_scaled.png
)

ADD_TEST(bfTeMeanShiftSegmentationFilterExampleTest ${EXE_TESTS}
        --compare-n-images ${NOTOL} 4
        ${BASELINE}/MSClusteredOutput.tif
        ${TEMP}/MSClusteredOutput.tif
        ${BASELINE}/MSLabeledOutput.tif
        ${TEMP}/MSLabeledOutput.tif
        ${BASELINE}/MSLabeledOutput-pretty.png
        ${TEMP}/MSLabeledOutput-pretty.png
        ${BASELINE}/MSClusteredOutput-pretty.png
        ${TEMP}/MSClusteredOutput-pretty.png
        MeanShiftSegmentationFilterExampleTest
        ${INPUTDATA}/ROI_QB_MUL_1.png
        ${TEMP}/MSLabeledOutput.tif
        ${TEMP}/MSClusteredOutput.tif
        ${TEMP}/MSLabeledOutput-pretty.png
        ${TEMP}/MSClusteredOutput-pretty.png
        16
        16
        100
        100
        0.1
)

# ------- HillShadingExampleTest ----------

ADD_TEST(bfTeHillShadingExampleTest ${EXE_TESTS}
        --compare-n-images ${NOTOL} 2
            ${BASELINE}/HillShadingExample.png
            ${TEMP}/HillShadingExample.png
                ${BASELINE}/HillShadingColorExample.png
            ${TEMP}/HillShadingColorExample.png
        HillShadingExampleTest
            ${TEMP}/HillShadingExample.png
            ${TEMP}/HillShadingColorExample.png
        6.8 45.3 500 500 0.002 -0.002 ${OTB_DATA_ROOT}/Examples/DEM_srtm
)

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbBasicFiltersExamplesTests otbBasicFiltersExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbBasicFiltersExamplesTests  ITKAlgorithms ITKStatistics OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
