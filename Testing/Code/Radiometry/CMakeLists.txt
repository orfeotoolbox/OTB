
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(RADIOMETRY_TESTS ${CXX_TEST_PATH}/otbRadiometryTests)

# -------            otb::RAndNIRVegetationIndexImageFilter   ------------------------------

ADD_TEST(raTuRAndNIRVegetationIndexImageFilterNew ${RADIOMETRY_TESTS}  
        otbRAndNIRVegetationIndexImageFilterNew )
        
ADD_TEST(raTvNDVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
                    ${TEMP}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
        otbRAndNIRVegetationIndexImageFilter
        NDVI
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${TEMP}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif 
	)
ADD_TEST(raTvRVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_RVI_poupees_subc1c2.tif
                    ${TEMP}/raRAndNIRVegetationIndex_RVI_poupees_subc1c2.tif
        otbRAndNIRVegetationIndexImageFilter
        RVI
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${TEMP}/raRAndNIRVegetationIndex_RVI_poupees_subc1c2.tif 
	)
ADD_TEST(raTvPVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_PVI_poupees_subc1c2.tif
                    ${TEMP}/raRAndNIRVegetationIndex_PVI_poupees_subc1c2.tif
        otbSetASetBRAndNIRVegetationIndexImageFilter
        PVI
        0.7
        0.9
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${TEMP}/raRAndNIRVegetationIndex_PVI_poupees_subc1c2.tif 
	)
ADD_TEST(raTvSAVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_SAVI_poupees_subc1c2.tif
                    ${TEMP}/raRAndNIRVegetationIndex_SAVI_poupees_subc1c2.tif
        otbRAndNIRVegetationIndexImageFilter
        SAVI
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${TEMP}/raRAndNIRVegetationIndex_SAVI_poupees_subc1c2.tif 
	)
ADD_TEST(raTvTSAVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_TSAVI_poupees_subc1c2.tif
                    ${TEMP}/raRAndNIRVegetationIndex_TSAVI_poupees_subc1c2.tif
        otbSetASetBRAndNIRVegetationIndexImageFilter
        TSAVI
        0.7
        0.9
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${TEMP}/raRAndNIRVegetationIndex_TSAVI_poupees_subc1c2.tif 
	)
ADD_TEST(raTvMSAVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_MSAVI_poupees_subc1c2.tif
                    ${TEMP}/raRAndNIRVegetationIndex_MSAVI_poupees_subc1c2.tif
        otbRAndNIRVegetationIndexImageFilter
        MSAVI
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${TEMP}/raRAndNIRVegetationIndex_MSAVI_poupees_subc1c2.tif 
	)

# -------            otb::MultiChannelRAndNIRVegetationIndexImageFilter   ------------------------------

ADD_TEST(raTuMultiChannelRAndNIRVegetationIndexImageFilterNew ${RADIOMETRY_TESTS}  
        otbMultiChannelRAndNIRVegetationIndexImageFilterNew )

ADD_TEST(raTvNDVI_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
                    ${TEMP}/raMultiChannelRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
        otbMultiChannelRAndNIRVegetationIndexImageFilter
        NDVI
	${INPUTDATA}/poupees_sub.png
	${TEMP}/raMultiChannelRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif 
	1 2
	)
ADD_TEST(raTvRVI_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_RVI_poupees_subc1c2.tif
                    ${TEMP}/raMultiChannelRAndNIRVegetationIndex_RVI_poupees_subc1c2.tif
        otbMultiChannelRAndNIRVegetationIndexImageFilter
        RVI
	${INPUTDATA}/poupees_sub.png
	${TEMP}/raMultiChannelRAndNIRVegetationIndex_RVI_poupees_subc1c2.tif 
	1 2
	)
ADD_TEST(raTvPVI_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_PVI_poupees_subc1c2.tif
                    ${TEMP}/raMultiChannelRAndNIRVegetationIndex_PVI_poupees_subc1c2.tif
        otbSetASetBMultiChannelRAndNIRVegetationIndexImageFilter
        PVI
        0.7
        0.9
	${INPUTDATA}/poupees_sub.png
	${TEMP}/raMultiChannelRAndNIRVegetationIndex_PVI_poupees_subc1c2.tif 
	1 2
        )
ADD_TEST(raTvSAVI_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_SAVI_poupees_subc1c2.tif
                    ${TEMP}/raMultiChannelRAndNIRVegetationIndex_SAVI_poupees_subc1c2.tif
        otbMultiChannelRAndNIRVegetationIndexImageFilter
        SAVI
	${INPUTDATA}/poupees_sub.png
	${TEMP}/raMultiChannelRAndNIRVegetationIndex_SAVI_poupees_subc1c2.tif 
	1 2
	)
ADD_TEST(raTvTSAVI_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_TSAVI_poupees_subc1c2.tif
                    ${TEMP}/raMultiChannelRAndNIRVegetationIndex_TSAVI_poupees_subc1c2.tif
        otbSetASetBMultiChannelRAndNIRVegetationIndexImageFilter
        TSAVI
        0.7
        0.9
	${INPUTDATA}/poupees_sub.png
	${TEMP}/raMultiChannelRAndNIRVegetationIndex_TSAVI_poupees_subc1c2.tif 
	1 2
	)
ADD_TEST(raTvMSAVI_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_MSAVI_poupees_subc1c2.tif
                    ${TEMP}/raMultiChannelRAndNIRVegetationIndex_MSAVI_poupees_subc1c2.tif
        otbMultiChannelRAndNIRVegetationIndexImageFilter
        MSAVI
	${INPUTDATA}/poupees_sub.png
	${TEMP}/raMultiChannelRAndNIRVegetationIndex_MSAVI_poupees_subc1c2.tif 
	1 2
	)


# -------            otb::RAndBAndNIRVegetationIndexImageFilter   ------------------------------

ADD_TEST(raTuRAndBAndNIRVegetationIndexImageFilterNew ${RADIOMETRY_TESTS}  
        otbRAndBAndNIRVegetationIndexImageFilterNew )


ADD_TEST(raTvARVI_RAndBAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/raRAndBAndNIRVegetationIndex_ARVI_poupees_subc1c2c3.tif
                    ${TEMP}/raRAndBAndNIRVegetationIndex_ARVI_poupees_subc1c2c3.tif
        otbRAndBAndNIRVegetationIndexImageFilter
        ARVI
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${INPUTDATA}/poupees_sub_c3.png
	${TEMP}/raRAndBAndNIRVegetationIndex_ARVI_poupees_subc1c2c3.tif 
        0.6 # Gamma parameter
	)

# -------            otb::MultiChannelRAndBAndNIRVegetationIndexImageFilter   ------------------------------

ADD_TEST(raTuMultiChannelRAndBAndNIRVegetationIndexImageFilterNew ${RADIOMETRY_TESTS}  
        otbMultiChannelRAndBAndNIRVegetationIndexImageFilterNew )

ADD_TEST(raTvARVI_MultiChannelRAndBAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/raRAndBAndNIRVegetationIndex_ARVI_poupees_subc1c2c3.tif
                    ${TEMP}/raMultiChannelRAndBAndNIRVegetationIndex_ARVI_poupees_subc1c2c3.tif
        otbMultiChannelRAndBAndNIRVegetationIndexImageFilter
        ARVI
	${INPUTDATA}/poupees_sub.png
	${TEMP}/raMultiChannelRAndBAndNIRVegetationIndex_ARVI_poupees_subc1c2c3.tif 
	1 2 3
        0.6 # Gamma parameter
	)
        
        

# A enrichir
SET(Radiometry_SRCS
otbRAndNIRVegetationIndexImageFilterNew.cxx
otbRAndNIRVegetationIndexImageFilter.cxx
otbSetASetBRAndNIRVegetationIndexImageFilter.cxx
otbMultiChannelRAndNIRVegetationIndexImageFilterNew.cxx
otbMultiChannelRAndNIRVegetationIndexImageFilter.cxx
otbSetASetBMultiChannelRAndNIRVegetationIndexImageFilter.cxx
otbRAndBAndNIRVegetationIndexImageFilterNew.cxx
otbRAndBAndNIRVegetationIndexImageFilter.cxx
otbMultiChannelRAndBAndNIRVegetationIndexImageFilterNew.cxx
otbMultiChannelRAndBAndNIRVegetationIndexImageFilter.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbRadiometryTests otbRadiometryTests.cxx ${Radiometry_SRCS})
TARGET_LINK_LIBRARIES(otbRadiometryTests OTBRadiometry OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




