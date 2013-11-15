PROJECT(ImageRegistrationExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ImageRegistration1 ImageRegistration1.cxx )
TARGET_LINK_LIBRARIES(ImageRegistration1 OTBCommon OTBIO ${ITK_LIBRARIES} )

ADD_EXECUTABLE(ImageRegistration2 ImageRegistration2.cxx )
TARGET_LINK_LIBRARIES(ImageRegistration2 OTBCommon OTBIO ${ITK_LIBRARIES} )

ADD_EXECUTABLE(ImageRegistration9 ImageRegistration9.cxx )
TARGET_LINK_LIBRARIES(ImageRegistration9 OTBCommon OTBIO ${ITK_LIBRARIES} )

ADD_EXECUTABLE(ImageRegistration5 ImageRegistration5.cxx )
TARGET_LINK_LIBRARIES(ImageRegistration5 OTBCommon OTBIO ${ITK_LIBRARIES} )

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Registration)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbRegistrationExamplesTests)

SET(TOL 0.0)

ADD_TEST(reTeImageRegistration1ExampleTest ${EXE_TESTS}
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

ADD_TEST(reTeImageRegistration2ExampleTest ${EXE_TESTS}
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

ADD_TEST(reTeImageRegistration5Cas1ExampleTest ${EXE_TESTS}
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
ADD_TEST(reTeImageRegistration5Cas2ExampleTest ${EXE_TESTS}
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

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbRegistrationExamplesTests otbRegistrationExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbRegistrationExamplesTests ${ITK_LIBRARIES} OTBCommon OTBIO OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
