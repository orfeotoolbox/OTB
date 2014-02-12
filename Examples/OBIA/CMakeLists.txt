project(OBIAExamples)
include_regular_expression("^.*$")

add_executable(ImageToLabelToImage ImageToLabelToImage.cxx )
target_link_libraries(ImageToLabelToImage OTBCommon OTBIO)


add_executable(ShapeAttributeComputation ShapeAttributeComputation.cxx )
target_link_libraries(ShapeAttributeComputation OTBCommon OTBIO)

add_executable(KeepNObjects KeepNObjects.cxx )
target_link_libraries(KeepNObjects OTBCommon OTBIO)

add_executable(RadiometricAttributesLabelMapFilterExample RadiometricAttributesLabelMapFilterExample.cxx )
target_link_libraries(RadiometricAttributesLabelMapFilterExample OTBCommon OTBIO OTBBasicFilters OTBSpatialReasoning  OTBFeatureExtraction OTBLearning)

add_executable(LabelMapToVectorData LabelMapToVectorData.cxx )
target_link_libraries(LabelMapToVectorData OTBCommon OTBIO OTBBasicFilters)

add_executable(HooverMetricsEstimation HooverMetricsEstimation.cxx )
target_link_libraries(HooverMetricsEstimation OTBCommon OTBIO OTBBasicFilters)

if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/OBIA)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

#Remote sensing images (large images )
if(OTB_DATA_USE_LARGEINPUT)
  set(INPUTLARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
endif()

set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbOBIAExamplesTests)
set(EXE_TESTS1 ${CXX_TEST_PATH}/otbOBIAExamplesTests1)

set(TOL 0.0)
set(EPS 0.001)

add_test( trTeOBIAImageToLabelToImageTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/OBIAI2L2IOutput.tif
                                        ${TEMP}/OBIAI2L2IOutput.tif
        OBIAImageToLabelToImageTest
        ${INPUTDATA}/MSLabeledOutput.tif
        ${TEMP}/OBIAI2L2IOutput.tif
        ${TEMP}/OBIAI2L2IInPretty.png
        ${TEMP}/OBIAI2L2IOutPretty.png
        1 1108 0
        )

add_test(trTeOBIAShapeAttributeComputationTest ${EXE_TESTS1}
        --compare-ascii ${EPS}        ${BASELINE}/OBIAShapeAttribute.txt
                                ${TEMP}/OBIAShapeAttribute.txt
        OBIAShapeAttributeComputationTest
        ${INPUTDATA}/MSLabeledOutput.tif
        ${TEMP}/OBIAShapeAttribute.txt
)

add_test(trTeOBIARadiometricAttributesLabelMapFilterTest ${EXE_TESTS1}
        --compare-image ${TOL}        ${BASELINE}/OBIARadiometricAttributes.png
                                ${TEMP}/OBIARadiometricAttributes.png
        RadiometricAttributesLabelMapFilterExample
        ${INPUTDATA}/qb_RoadExtract.tif
        ${TEMP}/OBIARadiometricAttributes.png
        ${TEMP}/qb_RoadExtract_Radiometry_pretty.png
        STATS::Band1::Mean
        0
        0.5
        16
        16
        50
        1.0
)

add_test(trTeLabelMapToVectorData ${EXE_TESTS}
        LabelMapToVectorDataTest
        ${INPUTDATA}/rcc8_mire1.png
        ${TEMP}/rcc8_mire2_vectorizer.shp
)
#need to add the test of radiometric labelmap filter

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbOBIAExamplesTests otbOBIAExamplesTests.cxx)
target_link_libraries(otbOBIAExamplesTests OTBCommon OTBIO OTBTesting)

add_executable(otbOBIAExamplesTests1 otbOBIAExamplesTests1.cxx)
target_link_libraries(otbOBIAExamplesTests1 ${ITK_LIBRARIES} OTBCommon OTBIO OTBTesting)


endif()
