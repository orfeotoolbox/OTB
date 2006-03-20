
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)
SET(IMAGEDATA ${OTB_DATA_DIR} )

#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(COMMON_TESTS ${CXX_TEST_PATH}/otbCommonTests)

#ADD_TEST(coTuCommandLineArgumentParser ${COMMON_TESTS}  
#        otbTestCommandLineArgumentParser
#        -image image.png -entier 150 -e 200 -double 12.25 -float 45.36 -deuxentiers 78 56 )


# -------            otb::ExtractROI   ------------------------------

ADD_TEST(coTvExtractROI ${COMMON_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
        otbExtractROI
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/coExtractROI_cthead1_26_97_209_100.png 
        26 97 209 100 )
ADD_TEST(coTvExtractROI_RGB ${COMMON_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/coExtractROI_RGB_sbuv_700_60_77_489.png
                    ${TEMP}/coExtractROI_RGB_sbuv_700_60_77_489.png
        otbExtractROI_RGB
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/coExtractROI_RGB_sbuv_700_60_77_489.png 
        700 60 77 489 )
ADD_TEST(coTvExtractROI_RGB2 ${COMMON_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/coExtractROI_RGB_poupees_302_1_134_330.jpg
                    ${TEMP}/coExtractROI_RGB_poupees_302_1_134_330.jpg
        otbExtractROI_RGB
        ${INPUTDATA}/poupees.jpg 
        ${TEMP}/coExtractROI_RGB_poupees_302_1_134_330.jpg 
        303 2 134 330 )


# -------            otb::MultiChannelExtractROI   ------------------------------

ADD_TEST(coTvMultiChannelROI_RGB2RGB_PNG ${COMMON_TESTS}   
  --compare-image ${TOL}   ${INPUTDATA}/sbuv.png
                           ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_sbuv.png
         otbMultiChannelExtractROI 
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_sbuv.png
         -channels 1 2 3 )
#         -sizeX 800 -sizeY 600 )

ADD_TEST(coTvMultiChannelROI_RGB2RGB_PNG2 ${COMMON_TESTS}   
  --compare-image ${TOL}   ${BASELINE}/coMultiChannelExtractROI_RGB2RGB_PNG_sbuv_700_60_77_489_channels_123.png
                           ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_sbuv_700_60_77_489_channels_123.png
         otbMultiChannelExtractROI 
         -uchar ${INPUTDATA}/sbuv.png 
         -uchar ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_sbuv_700_60_77_489_channels_123.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489 -channels 1 2 3  )
         
ADD_TEST(coTvMultiChannelROI_RGB2RGB_PNG3 ${COMMON_TESTS}   
  --compare-image ${TOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2RGB_PNG_sbuv_700_60_77_489_channels_321.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_sbuv_700_60_77_489_channels_321.png
         otbMultiChannelExtractROI 
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_sbuv_700_60_77_489_channels_321.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489 -channels 3 2 1  )
ADD_TEST(coTvMultiChannelROI_RGB2NG_PNG1 ${COMMON_TESTS}   
  --compare-image ${TOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_sbuv_700_60_77_489_channel_1.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_sbuv_700_60_77_489_channel_1.png
         otbMultiChannelExtractROI 
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_sbuv_700_60_77_489_channel_1.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489 -channels 1 )
ADD_TEST(coTvMultiChannelROI_RGB2NG_PNG2 ${COMMON_TESTS}   
  --compare-image ${TOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_sbuv_700_60_77_489_channel_2.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_sbuv_700_60_77_489_channel_2.png
         otbMultiChannelExtractROI 
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_sbuv_700_60_77_489_channel_2.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489 -channels 2 )
ADD_TEST(coTvMultiChannelROI_RGB2NG_PNG3 ${COMMON_TESTS}   
  --compare-image ${TOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_sbuv_700_60_77_489_channel_3.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_sbuv_700_60_77_489_channel_3.png
         otbMultiChannelExtractROI 
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_sbuv_700_60_77_489_channel_3.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489 -channels 3 )

ADD_TEST(coTvMultiChannelROI_5c_BSQ ${COMMON_TESTS}   
  --compare-image ${TOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_1232.hd
                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_1232.hd
         otbMultiChannelExtractROI 
         ${INPUTDATA}/poupees.hd 
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_1232.hd
         -startX 303 -startY 2 -sizeX 134  -sizeY 330 -channels 1 2 3 2 )

ADD_TEST(coTvMultiChannelROI_3c_BSQ ${COMMON_TESTS}   
  --compare-binary  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_1232.c3
                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_345.c1
         otbMultiChannelExtractROI 
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_1232.hd
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_345.hd
         -startX 0 -startY 0 -sizeX 134  -sizeY 330 -channels 3 4 5 )


# -------            otb::MultiToMonoChannelExtractROI   ------------------------------

ADD_TEST(coTvMultiToMonoROI_RGB2NG_PNG ${COMMON_TESTS}   
  --compare-image ${TOL}   ${BASELINE}/coMultiToMonoChannelExtractROI_RGB2NG_PNG_sbuv_700_60_77_489_channel_1.png
                           ${TEMP}/coMultiToMonoChannelExtractROI_RGB2NG_PNG_sbuv_700_60_77_489_channel_1.png
         otbMultiToMonoChannelExtractROI 
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/coMultiToMonoChannelExtractROI_RGB2NG_PNG_sbuv_700_60_77_489_channel_1.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489 -channel 1 )





# -------       Fichiers sources CXX -----------------------------------
SET(BasicCommon_SRCS
otbTestCommandLineArgumentParser.cxx
otbExtractROI.cxx
otbExtractROI_RGB.cxx
otbMultiChannelExtractROI.cxx
otbMultiToMonoChannelExtractROI.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

# ${TIFF_LIBRARY}

ADD_EXECUTABLE(otbCommonTests otbCommonTests.cxx ${BasicCommon_SRCS})
TARGET_LINK_LIBRARIES(otbCommonTests OTBIO OTBCommon ${CAI_LIBRARY}  ${GDAL_LIBRARY} ITKIO ITKCommon)

#ADD_EXECUTABLE(otbRescaleImage otbRescaleImage.cxx)
#TARGET_LINK_LIBRARIES(otbRescaleImage OTBIO OTBCommon ${CAI_LIBRARY}  ${GDAL_LIBRARY} ITKIO ITKCommon)



ENDIF( NOT OTB_DISABLE_CXX_TESTING )


# -----------------------------------------------------------------------
# Exemple test simple avec un "main"
# 
# ADD_EXECUTABLE(toto toto.cxx)
# TARGET_LINK_LIBRARIES(toto ITKIO ITKCommon)
# ADD_TEST(toto1 ${CXX_TEST_PATH}/toto 
#         ${INPUTDATA}/cthead1.png 
#         ${TEMP}/CoExtractROIImage.png 
#         ...... )

