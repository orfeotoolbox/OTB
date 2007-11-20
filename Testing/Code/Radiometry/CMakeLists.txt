
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.000000000001)

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
  --compare-image ${EPSILON}   ${BASELINE}/raRAndNIRVegetationIndex_PVI_poupees_subc1c2.tif
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
  --compare-image ${EPSILON}   ${BASELINE}/raRAndNIRVegetationIndex_TSAVI_poupees_subc1c2.tif
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
  --compare-image ${EPSILON}   ${BASELINE}/raRAndNIRVegetationIndex_PVI_poupees_subc1c2.tif
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
  --compare-image ${EPSILON}   ${BASELINE}/raRAndNIRVegetationIndex_TSAVI_poupees_subc1c2.tif
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
  --compare-image ${EPSILON}   ${BASELINE}/raRAndBAndNIRVegetationIndex_ARVI_poupees_subc1c2c3.tif
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
  --compare-image ${EPSILON}   ${BASELINE}/raRAndBAndNIRVegetationIndex_ARVI_poupees_subc1c2c3.tif
                    ${TEMP}/raMultiChannelRAndBAndNIRVegetationIndex_ARVI_poupees_subc1c2c3.tif
        otbMultiChannelRAndBAndNIRVegetationIndexImageFilter
        ARVI
	${INPUTDATA}/poupees_sub.png
	${TEMP}/raMultiChannelRAndBAndNIRVegetationIndex_ARVI_poupees_subc1c2c3.tif 
	1 2 3
        0.6 # Gamma parameter
	)
        
# -------            otb::ImageToLuminanceImageFilter   ------------------------------
ADD_TEST(raTuImageToLuminanceImageFilterNew ${RADIOMETRY_TESTS}  
        otbImageToLuminanceImageFilterNew
)

ADD_TEST(raTvImageToLuminanceImageFilter ${RADIOMETRY_TESTS} 
  --compare-image ${EPSILON}  ${BASELINE}/raTvImageToLuminanceImageFilter.tif
                          ${TEMP}/raTvImageToLuminanceImageFilter.tif
        otbImageToLuminanceImageFilter
	${INPUTDATA}/poupees_sub.png 
	${TEMP}/raTvImageToLuminanceImageFilter.tif
	10   #channel 1 alpha
	20   #channel 2 alpha	
	30   #channel 3 alpha	
	40   #channel 4 alpha
	1    #channel 1 beta
	2    #channel 2 beta
	3    #channel 3 beta
	4    #channel 4 beta
         ) 

# -------            otb::LuminanceToReflectanceImageFilter   ------------------------------
ADD_TEST(raTuLuminanceToReflectanceImageFilterNew ${RADIOMETRY_TESTS}  
        otbLuminanceToReflectanceImageFilterNew
)

ADD_TEST(raTvLuminanceToReflectanceImageFilter ${RADIOMETRY_TESTS} 
  --compare-image ${EPSILON}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterDsol.tif
                          ${TEMP}/raTvLuminanceToReflectanceImageFilterDsol.tif
        otbLuminanceToReflectanceImageFilter
	${INPUTDATA}/poupees_sub.png 
	${TEMP}/raTvLuminanceToReflectanceImageFilterDsol.tif
	0.2 #radius
	10  #channel 1 illumination
	20  #channel 2 illumination	
	30  #channel 3 illumination
	40  #channel 4 illumination
	0.9923885328 #d/d0 corresponding to the date 03/05
         ) 

ADD_TEST(raTvLuminanceToReflectanceImageFilterDayMounth ${RADIOMETRY_TESTS} 
  --compare-image ${EPSILON}  ${BASELINE}/raTvLuminanceToReflectanceImageFilter.tif
                          ${TEMP}/raTvLuminanceToReflectanceImageFilterDayMounth.tif
        otbLuminanceToReflectanceImageFilter
	${INPUTDATA}/poupees_sub.png 
	${TEMP}/raTvLuminanceToReflectanceImageFilterDayMounth.tif
	0.2 #radius
	10  #channel 1 illumination
	20  #channel 2 illumination	
	30  #channel 3 illumination
	40  #channel 4 illumination
	3   #day
	5   #mounth
        ) 



# -------            otb::ImageToReflectanceImageFilter   ------------------------------
ADD_TEST(raTuImageToReflectanceImageFilterNew ${RADIOMETRY_TESTS}  
        otbImageToReflectanceImageFilterNew
)

ADD_TEST(raTvImageToReflectanceImageFilter ${RADIOMETRY_TESTS} 
  --compare-image ${EPSILON}  ${BASELINE}/raTvImageToReflectanceImageFilterDsol.tif
                          ${TEMP}/raTvImageToReflectanceImageFilterDsol.tif
        otbImageToReflectanceImageFilter
	${INPUTDATA}/poupees_sub.png 
	${TEMP}/raTvImageToReflectanceImageFilterDsol.tif
	0.2 #radius
	1   #channel 1 alpha
	2   #channel 2 alpha	
	3   #channel 3 alpha	
	4   #channel 4 alpha
	10  #channel 1 beta
	11  #channel 2 beta
	12  #channel 3 beta
	13  #channel 4 beta
	10  #channel 1 illumination
	20  #channel 2 illumination	
	30  #channel 3 illumination
	40  #channel 4 illumination
	0.9923885328 #d/d0 corresponding to the date 03/05
         ) 

ADD_TEST(raTvImageToReflectanceImageFilterDayMounth ${RADIOMETRY_TESTS} 
  --compare-image ${EPSILON}  ${BASELINE}/raTvImageToReflectanceImageFilter.tif
                          ${TEMP}/raTvImageToReflectanceImageFilterDayMounth.tif
        otbImageToReflectanceImageFilter
	${INPUTDATA}/poupees_sub.png 
	${TEMP}/raTvImageToReflectanceImageFilterDayMounth.tif
	0.2 #radius
	1   #channel 1 alpha
	2   #channel 2 alpha	
	3   #channel 3 alpha	
	4   #channel 4 alpha
	10  #channel 1 beta
	11  #channel 2 beta
	12  #channel 3 beta
	13  #channel 4 beta
	10  #channel 1 illumination
	20  #channel 2 illumination	
	30  #channel 3 illumination
	40  #channel 4 illumination
	3   #day
	5   #mounth
       )

# -------            otb::DEMCaracteristicsExtractor   ------------------------------

ADD_TEST(raTuDEMCaracteristicsExtractorNew ${RADIOMETRY_TESTS}  
        otbDEMCaracteristicsExtractorNew 
)


ADD_TEST(raTvDEMCaracteristicsExtractor ${RADIOMETRY_TESTS}  
  --compare-n-images ${EPSILON} 4
                     ${BASELINE}/raTvDEMCaracteristicsExtractorSlop.tif
                     ${TEMP}/raTvDEMCaracteristicsExtractorSlop.tif
                     ${BASELINE}/raTvDEMCaracteristicsExtractorAspect.tif 
                     ${TEMP}/raTvDEMCaracteristicsExtractorAspect.tif 
		     ${BASELINE}/raTvDEMCaracteristicsExtractorIncidence.tif
                     ${TEMP}/raTvDEMCaracteristicsExtractorIncidence.tif
                     ${BASELINE}/raTvDEMCaracteristicsExtractorExitance.tif 
                     ${TEMP}/raTvDEMCaracteristicsExtractorExitance.tif 
        otbDEMCaracteristicsExtractor
	${INPUTDATA}/pyramide.tif
	20 # view azimut
	40 # view angle
	10 # solar azimut
	30 # solar angle
	${TEMP}/raTvDEMCaracteristicsExtractorSlop.tif
	${TEMP}/raTvDEMCaracteristicsExtractorAspect.tif 
	${TEMP}/raTvDEMCaracteristicsExtractorIncidence.tif 
	${TEMP}/raTvDEMCaracteristicsExtractorExitance.tif 
	)


# -------            otb::AtmosphericCorrectionParameters   ------------------------------
ADD_TEST(raTuAtmosphericCorrectionParametersNew ${RADIOMETRY_TESTS}  
        otbAtmosphericCorrectionParametersNew
)

# -------            otb::AtmosphericRadiativeTerms   ------------------------------
ADD_TEST(raTuAtmosphericRadiativeTermsNew ${RADIOMETRY_TESTS}  
        otbAtmosphericRadiativeTermsNew
)


# -------            otb::ReflectanceToSurfaceReflectanceImageFilter   ------------------------------
ADD_TEST(raTuReflectanceToSurfaceReflectanceImageFilterNew ${RADIOMETRY_TESTS}  
        otbReflectanceToSurfaceReflectanceImageFilterNew
)
ADD_TEST(raTvReflectanceToSurfaceReflectanceImageFilter ${RADIOMETRY_TESTS} 
  --compare-image ${EPSILON}  ${BASELINE}/raTvReflectanceToSurfaceReflectanceImageFilter.tif
                              ${TEMP}/raTvReflectanceToSurfaceReflectanceImageFilter.tif
        otbReflectanceToSurfaceReflectanceImageFilter
	${INPUTDATA}/poupees_sub.png 
	${TEMP}/raTvReflectanceToSurfaceReflectanceImageFilter.tif
	1  # intrinsic atmospheric reflectance	
	2  # spherical albedo of the atmosphere
	3  # ozone transmission
	4  # ozygen transmission	
	1  # water vapor transmission	
	2  # downward transmittance
	3  # upward transmittance
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
otbImageToLuminanceImageFilterNew.cxx
otbImageToLuminanceImageFilter.cxx
otbLuminanceToReflectanceImageFilterNew.cxx
otbLuminanceToReflectanceImageFilter.cxx
otbImageToReflectanceImageFilterNew.cxx
otbImageToReflectanceImageFilter.cxx
otbDEMCaracteristicsExtractorNew.cxx
otbDEMCaracteristicsExtractor.cxx
otbAtmosphericCorrectionParametersNew.cxx
otbAtmosphericRadiativeTermsNew.cxx
otbReflectanceToSurfaceReflectanceImageFilterNew.cxx
otbReflectanceToSurfaceReflectanceImageFilter.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbRadiometryTests otbRadiometryTests.cxx ${Radiometry_SRCS})
TARGET_LINK_LIBRARIES(otbRadiometryTests OTBRadiometry OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters otb6S)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




