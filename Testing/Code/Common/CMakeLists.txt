
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

SET(COMMON_TESTS ${CXX_TEST_PATH}/otbCommonTests)

ADD_TEST(otbExceptionTest ${COMMON_TESTS} otbExceptionTest)


ADD_TEST(otbConvertFormatImage ${COMMON_TESTS}  
#  --compare-image   ${BASELINE}/../../cthead1.png
#                    ${TEMP}/otbConvertFormatImage.png
        otbConvertFormatImage
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/otbConvertFormatImage.jpg )


ADD_TEST(otbExtractROIImage ${COMMON_TESTS}  
  --compare-image   ${BASELINE}/../../Input/cthead1_26_97_209_100.png
                    ${TEMP}/otbExtractROIImage.png
        otbExtractROIImage
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/otbExtractROIImage.png 
        26 97 209 100 )

ADD_TEST(otbExtractROIImageRGB ${COMMON_TESTS}  
  --compare-image   ${BASELINE}/../../Input/poupees_302_1_134_330.jpg
                    ${TEMP}/otbExtractROIImageRGB.jpg
        otbExtractROIImageRGB
        ${INPUTDATA}/poupees.jpg 
        ${TEMP}/otbExtractROIImageRGB.jpg 
        303 2 134 330 )


SET(BasicCommon_SRCS
otbExceptionTest.cxx
otbExtractROIImage.cxx
otbExtractROIImageRGB.cxx
otbConvertFormatImage.cxx
)

ADD_EXECUTABLE(ImageReadWrite ImageReadWrite.cxx)
TARGET_LINK_LIBRARIES(ImageReadWrite ITKCommon ITKIO )
ADD_TEST(ImageReadWrite ${CXX_TEST_PATH}/ImageReadWrite 
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/ImageReadWrite.png )

ADD_EXECUTABLE(ImageReadWriteRGB ImageReadWriteRGB.cxx)
TARGET_LINK_LIBRARIES(ImageReadWriteRGB ITKCommon ITKIO )
ADD_TEST(ImageReadWriteRGB ${CXX_TEST_PATH}/ImageReadWriteRGB 
        ${INPUTDATA}/poupees.jpg 
        ${TEMP}/ImageReadWriteRGB.jpg )

# Exemple test simple avec un "main"
# 
# ADD_EXECUTABLE(otbExtractROIImage otbExtractROIImage.cxx)
# TARGET_LINK_LIBRARIES(otbExtractROIImage ITKIO ITKCommon)
# ADD_TEST(otbExtractROIImage1 ${CXX_TEST_PATH}/otbExtractROIImage 
#         ${INPUTDATA}/cthead1.png 
#         ${TEMP}/otbExtractROIImage.png 
#         20 30 45 88 )




INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")


ADD_EXECUTABLE(otbCommonTests otbCommonTests.cxx ${BasicCommon_SRCS})
TARGET_LINK_LIBRARIES(otbCommonTests OTBCommon ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




