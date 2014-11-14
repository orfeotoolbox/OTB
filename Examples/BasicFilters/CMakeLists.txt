project(BasicFiltersExamples)
include_regular_expression("^.*$")

add_executable(LeeImageFilter LeeImageFilter.cxx )
target_link_libraries(LeeImageFilter OTBCommon OTBIO)

add_executable(FrostImageFilter FrostImageFilter.cxx )
target_link_libraries(FrostImageFilter OTBCommon OTBIO)

add_executable(DEMToRainbowExample DEMToRainbowExample.cxx )
target_link_libraries(DEMToRainbowExample OTBCommon OTBIO)

add_executable(ScalingFilterExample ScalingFilterExample.cxx )
target_link_libraries(ScalingFilterExample OTBCommon OTBIO)

add_executable(PrintableImageFilterExample PrintableImageFilterExample.cxx )
target_link_libraries(PrintableImageFilterExample OTBCommon OTBIO)

add_executable(IndexedToRGBExample IndexedToRGBExample.cxx )
target_link_libraries(IndexedToRGBExample OTBCommon OTBIO)

add_executable(MeanShiftSegmentationFilterExample MeanShiftSegmentationFilterExample.cxx )
target_link_libraries(MeanShiftSegmentationFilterExample OTBCommon OTBIO OTBBasicFilters OTBSegmentation)

add_executable(HillShadingExample HillShadingExample.cxx )
target_link_libraries(HillShadingExample OTBCommon OTBIO OTBBasicFilters)

add_executable(BandMathFilterExample BandMathFilterExample.cxx )
target_link_libraries(BandMathFilterExample OTBCommon OTBIO OTBBasicFilters)

add_executable(BandMathFilterXExample BandMathFilterXExample.cxx )
target_link_libraries(BandMathFilterXExample OTBCommon OTBIO OTBBasicFilters)

if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/BasicFilters)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbBasicFiltersExamplesTests)

set(NOTOL 0.0)


# ------- LeeImageFilterTest----------

add_test(bfTeLeeImageFilterTest ${EXE_TESTS}
        --compare-n-images ${NOTOL} 1
        ${BASELINE}/GomaSmallLeeFiltered.png
        ${TEMP}/GomaSmallLeeFiltered.png
        LeeImageFilterTest
        ${INPUTDATA}/GomaSmall.png
        ${TEMP}/GomaSmallLeeFiltered.png
    3 1
)

# ------- FrostImageFilterTest----------

add_test(bfTeFrostImageFilterTest ${EXE_TESTS}
        --compare-n-images ${NOTOL} 1
        ${BASELINE}/GomaSmallFrostFiltered.png
        ${TEMP}/GomaSmallFrostFiltered.png
        FrostImageFilterTest
        ${INPUTDATA}/GomaSmall.png
        ${TEMP}/GomaSmallFrostFiltered.png
    5 0.1
)

# ------- DEMToRainbowExampleTest ----------

add_test(bfTeDEMToRainbowExampleTest ${EXE_TESTS}
        --compare-n-images ${NOTOL} 1
        ${BASELINE}/DEMToRainbowImageGenerator.png
        ${TEMP}/DEMToRainbowImageGenerator.png
        DEMToRainbowExampleTest
        ${TEMP}/DEMToRainbowImageGenerator.png
    6.5 45.5 500 500 0.002 -0.002 ${OTB_DATA_ROOT}/Examples/DEM_srtm
)


# ------- ScalingFilterExampleTest ----------

add_test(bfTeScalingFilterExampleTest ${EXE_TESTS}
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

add_test(bfTePrintableImageFilterExample1Test ${EXE_TESTS}
        --compare-n-images ${NOTOL} 1
        ${BASELINE}/PrintableExampleOutput1.png
        ${TEMP}/PrintableExampleOutput1.png
        PrintableImageFilterExample1Test
        ${INPUTDATA}/IMAGERY_SSECH.tif
        ${TEMP}/PrintableExampleOutput1.png
    1 2 3
)

add_test(bfTePrintableImageFilteExample2rTest ${EXE_TESTS}
        --compare-n-images ${NOTOL} 1
        ${BASELINE}/PrintableExampleOutput2.png
        ${TEMP}/PrintableExampleOutput2.png
        PrintableImageFilterExample2Test
        ${INPUTDATA}/IMAGERY_SSECH.tif
        ${TEMP}/PrintableExampleOutput2.png
    1 4 2
)

# ------- IndexedToRGBExampleTest----------

add_test(bfTeIndexedToRGBExampleTest ${EXE_TESTS}
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

add_test(bfTeMeanShiftSegmentationFilterExampleTest ${EXE_TESTS}
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

add_test(bfTeHillShadingExampleTest ${EXE_TESTS}
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

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbBasicFiltersExamplesTests otbBasicFiltersExamplesTests.cxx)
target_link_libraries(otbBasicFiltersExamplesTests   ${ITK_LIBRARIES} OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

endif()
