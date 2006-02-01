
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE_IMAGE ${OTB_DATA_ROOT}/Baseline/Images)
SET(BASELINE ${OTB_DATA_ROOT}/Baseline)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

SET(COMMON_TESTS ${CXX_TEST_PATH}/otbCommonTests)

# -------            otb::VectorToRGBImageFilter   ------------------------------
ADD_TEST(tvCoVectorToRGBImageFilter ${COMMON_TESTS}   
  --compare-image   ${BASELINE_IMAGE}/CoVectorToRGBImageFilter_sbuv.png
                    ${TEMP}/CoVectorToRGBImageFilter_sbuv.png
         otbVectorToRGBImageFilter 
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/CoVectorToRGBImageFilter_sbuv.png
         1 2 3 )

ADD_TEST(tvCoVectorToRGBImageFilter2 ${COMMON_TESTS}   
  --compare-image   ${BASELINE_IMAGE}/CoVectorToRGBImageFilter2_sbuv.png
                    ${TEMP}/CoVectorToRGBImageFilter2_sbuv.png
         otbVectorToRGBImageFilter 
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/CoVectorToRGBImageFilter2_sbuv.png
         3 2 1 )

# -------            otb::VectorToScalarImageFilter   ------------------------------
ADD_TEST(tvCoVectorToScalarImageFilter ${COMMON_TESTS}  
  --compare-image   ${BASELINE_IMAGE}/CoVectorToScalarImageFilter1_sbuv.png
                    ${TEMP}/CoVectorToScalarImageFilter1_sbuv.png
         otbVectorToScalarImageFilter
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/CoVectorToScalarImageFilter1_sbuv.png
         1 )
ADD_TEST(tvCoVectorToScalarImageFilter2 ${COMMON_TESTS}  
  --compare-image   ${BASELINE_IMAGE}/CoVectorToScalarImageFilter2_sbuv.png
                    ${TEMP}/CoVectorToScalarImageFilter2_sbuv.png
         otbVectorToScalarImageFilter
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/CoVectorToScalarImageFilter2_sbuv.png
         2 )
ADD_TEST(tvCoVectorToScalarImageFilter3 ${COMMON_TESTS}  
  --compare-image   ${BASELINE_IMAGE}/CoVectorToScalarImageFilter3_sbuv.png
                    ${TEMP}/CoVectorToScalarImageFilter3_sbuv.png
         otbVectorToScalarImageFilter
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/CoVectorToScalarImageFilter3_sbuv.png
         3 )

# -------            otb::ExtractROI   ------------------------------
ADD_TEST(tvCoExtractROI ${COMMON_TESTS}  
  --compare-image   ${BASELINE_IMAGE}/CoExtractROI_cthead1_26_97_209_100.png
                    ${TEMP}/CoExtractROI_cthead1_26_97_209_100.png
        otbExtractROI
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/CoExtractROI_cthead1_26_97_209_100.png 
        26 97 209 100 )
ADD_TEST(tvCoExtractROI_RGB ${COMMON_TESTS}  
  --compare-image   ${BASELINE_IMAGE}/CoExtractROI_RGB_sbuv_700_60_77_489.png
                    ${TEMP}/CoExtractROI_RGB_sbuv_700_60_77_489.png
        otbExtractROI_RGB
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/CoExtractROI_RGB_sbuv_700_60_77_489.png 
        700 60 77 489 )
ADD_TEST(tvCoExtractROI_RGB2 ${COMMON_TESTS}  
  --compare-image   ${BASELINE_IMAGE}/CoExtractROI_RGB_poupees_302_1_134_330.jpg
                    ${TEMP}/CoExtractROI_RGB_poupees_302_1_134_330.jpg
        otbExtractROI_RGB
        ${INPUTDATA}/poupees.jpg 
        ${TEMP}/CoExtractROI_RGB_poupees_302_1_134_330.jpg 
        303 2 134 330 )

# -------            otb::ExtractROI   ------------------------------
# -------            otb::VectorToScalarImageFilter   ------------------------------
ADD_TEST(tvCoExtractROI_VectorToScalar1 ${COMMON_TESTS} 
  --compare-image   ${BASELINE_IMAGE}/CoExtractROI_VectorToScalar_sbuv_700_60_77_489_channel_1.png
                    ${TEMP}/CoExtractROI_VectorToScalar_sbuv_700_60_77_489_channel_1.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/CoExtractROI_VectorToScalar_sbuv_700_60_77_489_channel_1.png
         700 60 77 489
         1 )
ADD_TEST(tvCoExtractROI_VectorToScalar2 ${COMMON_TESTS} 
  --compare-image   ${BASELINE_IMAGE}/CoExtractROI_VectorToScalar_sbuv_700_60_77_489_channel_2.png
                    ${TEMP}/CoExtractROI_VectorToScalar_sbuv_700_60_77_489_channel_2.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/CoExtractROI_VectorToScalar_sbuv_700_60_77_489_channel_2.png
         700 60 77 489
         2 )
ADD_TEST(tvCoExtractROI_VectorToScalar3 ${COMMON_TESTS} 
  --compare-image   ${BASELINE_IMAGE}/CoExtractROI_VectorToScalar_sbuv_700_60_77_489_channel_3.png
                    ${TEMP}/CoExtractROI_VectorToScalar_sbuv_700_60_77_489_channel_3.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/CoExtractROI_VectorToScalar_sbuv_700_60_77_489_channel_3.png
         700 60 77 489
         3 )

# -------            otb::ExtractROI   ------------------------------
# -------            otb::VectorToRGBImageFilter   ------------------------------
ADD_TEST(tvCoExtractROI_VectorToRGB ${COMMON_TESTS} 
  --compare-image   ${BASELINE_IMAGE}/CoExtractROI_VectorToRGB_sbuv_700_60_77_489_channels_1_2_3.png
                    ${TEMP}/CoExtractROI_VectorToRGB_sbuv_700_60_77_489_channels_1_2_3.png
         otbExtractROI_VectorToRGB
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/CoExtractROI_VectorToRGB_sbuv_700_60_77_489_channels_1_2_3.png
         700 60 77 489
         1 2 3 )








# -------       Fichiers sources CXX -----------------------------------
SET(BasicCommon_SRCS
otbExtractROI.cxx
otbExtractROI_RGB.cxx
otbVectorToRGBImageFilter.cxx
otbVectorToScalarImageFilter.cxx
otbExtractROI_VectorToScalar.cxx
otbExtractROI_VectorToRGB.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")


# ${TIFF_LIBRARY}

ADD_EXECUTABLE(otbCommonTests otbCommonTests.cxx ${BasicCommon_SRCS})
TARGET_LINK_LIBRARIES(otbCommonTests OTBIO OTBCommon ${CAI_LIBRARY}   ITKIO ITKCommon)




ENDIF( NOT OTB_DISABLE_CXX_TESTING )


# -----------------------------------------------------------------------
# Exemple test simple avec un "main"
# 
# ADD_EXECUTABLE(otbExtractROIImage otbExtractROIImage.cxx)
# TARGET_LINK_LIBRARIES(otbExtractROIImage ITKIO ITKCommon)
# ADD_TEST(otbExtractROIImage1 ${CXX_TEST_PATH}/CoExtractROIImage 
#         ${INPUTDATA}/cthead1.png 
#         ${TEMP}/CoExtractROIImage.png 
#         20 30 45 88 )

# ---------    TESTS A VALIDER OU SUPPRIMER -------------
#otbMultiChannelExtractROI.cxx
#otbMultiChannelExtractROI2.cxx
#otbMultiChannelExtractROI3.cxx
#otbMultiToMonoChannelExtractROI.cxx


#ADD_TEST(tvCoMultiChannelExtractROI ${COMMON_TESTS}  
#  --compare-ascii   ${BASELINE}/CoMultiChannelExtractROI.txt
#                    ${TEMP}/CoMultiChannelExtractROI.txt
#        otbMultiChannelExtractROI
#        ${TEMP}/CoMultiChannelExtractROI.txt )

#ADD_TEST(tvCoMultiChannelExtractROI2 ${COMMON_TESTS}  
#  --compare-ascii   ${BASELINE}/CoMultiChannelExtractROI2.txt
#                    ${TEMP}/CoMultiChannelExtractROI2.txt
#        otbMultiChannelExtractROI2
#        ${TEMP}/CoMultiChannelExtractROI2.txt )

#ADD_TEST(tvCoMultiToMonoChannelExtractROI ${COMMON_TESTS}  
#  --compare-ascii   ${BASELINE}/CoMultiToMonoChannelExtractROI.txt
#                    ${TEMP}/CoMultiToMonoChannelExtractROI.txt
#        otbMultiToMonoChannelExtractROI
#        ${TEMP}/CoMultiToMonoChannelExtractROI.txt )

#ADD_TEST(tvCoMultiChannelExtractROI3 ${COMMON_TESTS}  
#  --compare-image   ${BASELINE_IMAGE}/CoExtractROI_RGB_sbuv_700_60_77_489.png
#                    ${TEMP}/CoMultiChannelExtractROI_RGB_sbuv_700_60_77_489.png
#        otbMultiChannelExtractROI3
#        ${INPUTDATA}/sbuv.png 
#        ${TEMP}/CoMultiChannelExtractROI_RGB_sbuv_700_60_77_489.png 
#        700 60 77 489 )

