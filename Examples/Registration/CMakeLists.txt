project(ImageRegistrationExamples)
include_regular_expression("^.*$")

add_executable(ImageRegistration1 ImageRegistration1.cxx )
target_link_libraries(ImageRegistration1 OTBCommon OTBIO ${ITK_LIBRARIES} )

add_executable(ImageRegistration2 ImageRegistration2.cxx )
target_link_libraries(ImageRegistration2 OTBCommon OTBIO ${ITK_LIBRARIES} )

add_executable(ImageRegistration9 ImageRegistration9.cxx )
target_link_libraries(ImageRegistration9 OTBCommon OTBIO ${ITK_LIBRARIES} )

add_executable(ImageRegistration5 ImageRegistration5.cxx )
target_link_libraries(ImageRegistration5 OTBCommon OTBIO ${ITK_LIBRARIES} )

if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Registration)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbRegistrationExamplesTests)

set(TOL 0.0)

add_test(reTeImageRegistration1ExampleTest ${EXE_TESTS}
        --compare-image ${TOL}
        ${BASELINE}/ImageRegistration1Output.png
        ${TEMP}/ImageRegistration1Output.png
        ImageRegistration1ExampleTest
        ${INPUTDATA}/QB_Suburb.png
        ${INPUTDATA}/QB_Suburb13x17y.png
        ${TEMP}/ImageRegistration1Output.png
        ${TEMP}/ImageRegistration1DifferenceAfter.png
        ${TEMP}/ImageRegistration1DifferenceBefore.png
)

add_test(reTeImageRegistration2ExampleTest ${EXE_TESTS}
        --compare-image ${TOL}
        ${BASELINE}/ImageRegistration2Output.png
        ${TEMP}/ImageRegistration2Output.png
        ImageRegistration2ExampleTest
        ${INPUTDATA}/RamsesROISmall.png
        ${INPUTDATA}/ADS40RoiSmall.png
        ${TEMP}/ImageRegistration2Output.png
        ${TEMP}/ImageRegistration2CheckerboardBefore.png
        ${TEMP}/ImageRegistration2CheckerboardAfter.png
)

add_test(reTeImageRegistration5Cas1ExampleTest ${EXE_TESTS}
        --compare-image ${TOL}
        ${BASELINE}/ImageRegistration5Output1.png
        ${TEMP}/ImageRegistration5Output1.png
        ImageRegistration5ExampleTest
        ${INPUTDATA}/QB_Suburb.png
        ${INPUTDATA}/QB_SuburbRotated10.png
        ${TEMP}/ImageRegistration5Output1.png
        ${TEMP}/ImageRegistration5DifferenceBefore1.png
        ${TEMP}/ImageRegistration5DifferenceAfter1.png
)
add_test(reTeImageRegistration5Cas2ExampleTest ${EXE_TESTS}
        --compare-image ${TOL}
        ${BASELINE}/ImageRegistration5Output2.png
        ${TEMP}/ImageRegistration5Output2.png
        ImageRegistration5ExampleTest
        ${INPUTDATA}/QB_Suburb.png
        ${INPUTDATA}/QB_SuburbR10X13Y17.png
        ${TEMP}/ImageRegistration5Output2.png
        ${TEMP}/ImageRegistration5DifferenceBefore2.png
        ${TEMP}/ImageRegistration5DifferenceAfter2.png
)

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbRegistrationExamplesTests otbRegistrationExamplesTests.cxx)
target_link_libraries(otbRegistrationExamplesTests ${ITK_LIBRARIES} OTBCommon OTBIO OTBTesting)

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
