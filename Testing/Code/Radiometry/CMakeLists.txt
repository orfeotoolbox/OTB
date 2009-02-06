
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.000000000001)
SET(EPSILON_10e6 0.000001)
SET(EPSILON_10e10 0.0000000001)
SET(RADIOMETRY_TESTS1 ${CXX_TEST_PATH}/otbRadiometryTests1)
SET(RADIOMETRY_TESTS2 ${CXX_TEST_PATH}/otbRadiometryTests2)
SET(RADIOMETRY_TESTS3 ${CXX_TEST_PATH}/otbRadiometryTests3)
SET(RADIOMETRY_TESTS4 ${CXX_TEST_PATH}/otbRadiometryTests4)
SET(RADIOMETRY_TESTS5 ${CXX_TEST_PATH}/otbRadiometryTests5)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADIOMETRY_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::RAndNIRVegetationIndexImageFilter   ------------------------------

ADD_TEST(raTuRAndNIRVegetationIndexImageFilterNew ${RADIOMETRY_TESTS1}  
        otbRAndNIRVegetationIndexImageFilterNew )
        
ADD_TEST(raTvNDVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS1}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
                    ${TEMP}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
        otbRAndNIRVegetationIndexImageFilter
        NDVI
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${TEMP}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif 
	)
ADD_TEST(raTvRVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS1}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_RVI_poupees_subc1c2.tif
                    ${TEMP}/raRAndNIRVegetationIndex_RVI_poupees_subc1c2.tif
        otbRAndNIRVegetationIndexImageFilter
        RVI
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${TEMP}/raRAndNIRVegetationIndex_RVI_poupees_subc1c2.tif 
	)
ADD_TEST(raTvPVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS1}  
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
ADD_TEST(raTvSAVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS1}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_SAVI_poupees_subc1c2.tif
                    ${TEMP}/raRAndNIRVegetationIndex_SAVI_poupees_subc1c2.tif
        otbRAndNIRVegetationIndexImageFilter
        SAVI
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${TEMP}/raRAndNIRVegetationIndex_SAVI_poupees_subc1c2.tif 
	)
ADD_TEST(raTvTSAVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS1}  
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
ADD_TEST(raTvMSAVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS1}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_MSAVI_poupees_subc1c2.tif
                    ${TEMP}/raRAndNIRVegetationIndex_MSAVI_poupees_subc1c2.tif
        otbRAndNIRVegetationIndexImageFilter
        MSAVI
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${TEMP}/raRAndNIRVegetationIndex_MSAVI_poupees_subc1c2.tif 
	)

# -------            otb::MultiChannelRAndNIRVegetationIndexImageFilter   ------------------------------

ADD_TEST(raTuMultiChannelRAndNIRVegetationIndexImageFilterNew ${RADIOMETRY_TESTS1}  
        otbMultiChannelRAndNIRVegetationIndexImageFilterNew )

ADD_TEST(raTvNDVI_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS1}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
                    ${TEMP}/raMultiChannelRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
        otbMultiChannelRAndNIRVegetationIndexImageFilter
        NDVI
	${INPUTDATA}/poupees_sub.png
	${TEMP}/raMultiChannelRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif 
	1 2
	)
ADD_TEST(raTvRVI_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS1}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_RVI_poupees_subc1c2.tif
                    ${TEMP}/raMultiChannelRAndNIRVegetationIndex_RVI_poupees_subc1c2.tif
        otbMultiChannelRAndNIRVegetationIndexImageFilter
        RVI
	${INPUTDATA}/poupees_sub.png
	${TEMP}/raMultiChannelRAndNIRVegetationIndex_RVI_poupees_subc1c2.tif 
	1 2
	)
ADD_TEST(raTvPVI_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS1}  
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
ADD_TEST(raTvSAVI_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS1}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_SAVI_poupees_subc1c2.tif
                    ${TEMP}/raMultiChannelRAndNIRVegetationIndex_SAVI_poupees_subc1c2.tif
        otbMultiChannelRAndNIRVegetationIndexImageFilter
        SAVI
	${INPUTDATA}/poupees_sub.png
	${TEMP}/raMultiChannelRAndNIRVegetationIndex_SAVI_poupees_subc1c2.tif 
	1 2
	)
ADD_TEST(raTvTSAVI_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS1}  
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
ADD_TEST(raTvMSAVI_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS1}  
  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_MSAVI_poupees_subc1c2.tif
                    ${TEMP}/raMultiChannelRAndNIRVegetationIndex_MSAVI_poupees_subc1c2.tif
        otbMultiChannelRAndNIRVegetationIndexImageFilter
        MSAVI
	${INPUTDATA}/poupees_sub.png
	${TEMP}/raMultiChannelRAndNIRVegetationIndex_MSAVI_poupees_subc1c2.tif 
	1 2
	)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADIOMETRY_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::RAndBAndNIRVegetationIndexImageFilter   ------------------------------

ADD_TEST(raTuRAndBAndNIRVegetationIndexImageFilterNew ${RADIOMETRY_TESTS2}  
        otbRAndBAndNIRVegetationIndexImageFilterNew )


ADD_TEST(raTvARVI_RAndBAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS2}  
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

ADD_TEST(raTuMultiChannelRAndBAndNIRVegetationIndexImageFilterNew ${RADIOMETRY_TESTS2}  
        otbMultiChannelRAndBAndNIRVegetationIndexImageFilterNew )

ADD_TEST(raTvARVI_MultiChannelRAndBAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS2}  
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
ADD_TEST(raTuImageToLuminanceImageFilterNew ${RADIOMETRY_TESTS2}  
        otbImageToLuminanceImageFilterNew
)

ADD_TEST(raTvImageToLuminanceImageFilter ${RADIOMETRY_TESTS2} 
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
ADD_TEST(raTuLuminanceToReflectanceImageFilterNew ${RADIOMETRY_TESTS2}  
        otbLuminanceToReflectanceImageFilterNew
)

ADD_TEST(raTvLuminanceToReflectanceImageFilter ${RADIOMETRY_TESTS2} 
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

ADD_TEST(raTvLuminanceToReflectanceImageFilterDayMonth ${RADIOMETRY_TESTS2} 
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
ADD_TEST(raTuImageToReflectanceImageFilterNew ${RADIOMETRY_TESTS2}  
        otbImageToReflectanceImageFilterNew
)

ADD_TEST(raTvImageToReflectanceImageFilter ${RADIOMETRY_TESTS2} 
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

ADD_TEST(raTvRomaniaImageToReflectance ${RADIOMETRY_TESTS2} 
  --compare-image ${EPSILON}  
		  ${BASELINE}/raTvRomania_Reflectance.tif
                  ${TEMP}/raTvRomaniaReflectanceImage.tif
        otbImageToReflectanceImageFilter
	${INPUTDATA}/Romania_Extract.tif 
	${TEMP}/raTvRomaniaReflectanceImage.tif
	27.3                 # = 90-62.70 : elevation et azimuth solaire 
        1.981247824          #channel 1 alpha = 0.881338*2.24800 
	4.332207085          #channel 2 alpha = 0.858713*5.04500	
	2.32064768           #channel 3 alpha = 0.685568*3.38500 	
	9.3177861            #channel 4 alpha = 6.19122*1.50500
	0                    #channel 1 beta
	0                    #channel 2 beta
	0                    #channel 3 beta
	0                    #channel 4 beta
	1057.56              #channel 1 illumination
	1570.23              #channel 2 illumination    
	1842.94              #channel 3 illumination
	232.820              #channel 4 illumination
	0.9889145564708814   #= sqrt(0.977952) d/d0 corresponding to the date 03/05 
        ) 

ADD_TEST(raTvImageToReflectanceImageFilterDayMounth ${RADIOMETRY_TESTS2} 
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

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADIOMETRY_TESTS3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::DEMCaracteristicsExtractor   ------------------------------

ADD_TEST(raTuDEMCaracteristicsExtractorNew ${RADIOMETRY_TESTS3}  
        otbDEMCaracteristicsExtractorNew 
)


ADD_TEST(raTvDEMCaracteristicsExtractor ${RADIOMETRY_TESTS3}  
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
# launch the FilterFunctionValues unitary test too.
ADD_TEST(raTuAtmosphericCorrectionParametersNew ${RADIOMETRY_TESTS3}  
        otbAtmosphericCorrectionParametersNew
)

##########           otb::FilterFunctionValues -------------------
# in otbAtmosphericCorrectionParameters.h/.cxx
ADD_TEST(raTvFilterFunctionValuesTest ${RADIOMETRY_TESTS3}
    --compare-ascii ${EPSILON_10e6} ${BASELINE_FILES}/raTvFilterFunctionValuesTest.txt
                           ${TEMP}/raTvFilterFunctionValuesTest.txt
        otbFilterFunctionValuesTest
	${TEMP}/raTvFilterFunctionValuesTest.txt
	0.5             # lambda min
	0.7             # lambda max
	0.05            # user step
	0.2 0.5 0.3 0.9 0.8 # vector
)


##########           otb::SIXSTraits -------------------
ADD_TEST(raTvSIXSTraitsComputeAtmosphericParametersTest ${RADIOMETRY_TESTS3}
    --compare-ascii ${EPSILON_10e6} ${BASELINE_FILES}/raTvSIXSTraitsComputeAtmosphericParametersTest.txt
                           ${TEMP}/raTvSIXSTraitsComputeAtmosphericParametersTest.txt
        otbSIXSTraitsComputeAtmosphericParametersTest
	${INPUTDATA}/in6S_otb
	${TEMP}/raTvSIXSTraitsComputeAtmosphericParametersTest.txt
)



# -------            otb::AtmosphericRadiativeTerms   ------------------------------
ADD_TEST(raTuAtmosphericRadiativeTermsNew ${RADIOMETRY_TESTS3}  
        otbAtmosphericRadiativeTermsNew
)


# -------            otb::ReflectanceToSurfaceReflectanceImageFilter   ------------------------------
ADD_TEST(raTuReflectanceToSurfaceReflectanceImageFilterNew ${RADIOMETRY_TESTS3}  
        otbReflectanceToSurfaceReflectanceImageFilterNew
)
ADD_TEST(raTvReflectanceToSurfaceReflectanceImageFilter ${RADIOMETRY_TESTS3} 
  --compare-image ${EPSILON}  ${BASELINE}/raTvReflectanceToSurfaceReflectanceImageFilter.tif
                              ${TEMP}/raTvReflectanceToSurfaceReflectanceImageFilter.tif
        otbReflectanceToSurfaceReflectanceImageFilter
	${INPUTDATA}/poupees_sub.png 
	${TEMP}/raTvReflectanceToSurfaceReflectanceImageFilter.tif
	# SAME VALUE FOR EACH CHANNEL
	1 1 1 1 # intrinsic atmospheric reflectance	
	2 2 2 2 # spherical albedo of the atmosphere
	3 3 3 3 # total transmission	
	2 2 2 2 # downward transmittance
	3 3 3 3 # upward transmittance
       )

ADD_TEST(raTvRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter ${RADIOMETRY_TESTS3} 
  --compare-image ${EPSILON}   
		  ${BASELINE}/raTvRomania_Correction.tif
		  ${TEMP}/raTvRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter.tif
        otbRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter
		${BASELINE}/raTvRomania_Reflectance.tif 
		${TEMP}/raTvRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter.tif
		${INPUTDATA}/romania_parameter.txt                           # atmo param;
		${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B3.txt   # wavelenghts, channel 3	
		${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B2.txt   # wavelenghts, channel 2	
		${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B1.txt   # wavelenghts, channel 1	
		${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_MIR.txt  # wavelenghts, channel 4
      )

# -------            otb::SurfaceAdjencyEffect6SCorrectionSchemeFilter   ------------------------------
ADD_TEST(raTuSurfaceAdjencyEffect6SCorrectionSchemeFilterNew ${RADIOMETRY_TESTS3}  
        otbSurfaceAdjencyEffect6SCorrectionSchemeFilterNew
)

ADD_TEST(raTvSurfaceAdjencyEffect6SCorrectionSchemeFilter ${RADIOMETRY_TESTS3}  
 --compare-image ${EPSILON}  ${BASELINE}/raTvSurfaceAdjencyEffect6SCorrectionSchemeFilter.tif
                             ${TEMP}/raTvSurfaceAdjencyEffect6SCorrectionSchemeFilter.tif
# --compare-ascii ${EPSILON_10e6} ${BASELINE_FILES}/raTvSurfaceAdjencyEffect6SCorrectionSchemeFilterOutput6SVallues.txt
#                           ${TEMP}/raTvSurfaceAdjencyEffect6SCorrectionSchemeFilterOutput6SVallues.txt
        otbSurfaceAdjencyEffect6SCorrectionSchemeFilter
	${BASELINE}/raTvRomania_Correction.tif
	${TEMP}/raTvSurfaceAdjencyEffect6SCorrectionSchemeFilter.tif
	2                                                            # Radius;	
	0.020                                                        # pixel spacing in kilometers
	${INPUTDATA}/romania_parameter.txt                           # atmo param;
	${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B3.txt   # wavelenghts, channel 3	
	${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B2.txt   # wavelenghts, channel 2	
	${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B1.txt   # wavelenghts, channel 1	
	${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_MIR.txt  # wavelenghts, channel 4
	# FOR plateform verification only : to remove when OK		     
	${TEMP}/raTvSurfaceAdjencyEffect6SCorrectionSchemeFilterOutput6SVallues.txt
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADIOMETRY_TESTS4 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms   ------------------------------
ADD_TEST(raTuAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTermsNew ${RADIOMETRY_TESTS4}  
        otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTermsNew)

ADD_TEST(raTvAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms ${RADIOMETRY_TESTS4}  
--compare-ascii ${EPSILON_10e6}  
		${BASELINE_FILES}/raTvSIXSTraitsComputeAtmosphericParametersTest.txt
                ${TEMP}/raTvCorrectionTo6SRadiative.txt
        otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
                ${INPUTDATA}/in6S_otb
                ${TEMP}/raTvCorrectionTo6SRadiative.txt
)

ADD_TEST(raTvAtmosphericCorrectionSequencementTest ${RADIOMETRY_TESTS4}  
        --compare-image ${EPSILON_10e10}  
		${BASELINE}/raTvRomaniaAtmosphericCorrectionSequencement.tif
                ${TEMP}/raTvRomaniaAtmosphericCorrectionSequencement.tif
        otbAtmosphericCorrectionSequencementTest
        ${INPUTDATA}/Romania_Extract.tif
        ${TEMP}/raTvRomaniaAtmosphericCorrectionSequencement.tif
# ImageToLuminance parameters                
        ${INPUTDATA}/atmosphericCorrectionSequencement_alpha_beta.txt
# LuminanceToReflectance parameters                
        27.3    #elevation et azimuth solaire
	4       #day
	12      #month
        ${INPUTDATA}/atmosphericCorrectionSequencement_solar_illumination.txt
# AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms parameters                
        152.7
        2.5
        -77.0
        1013.
        2.48134
        0.34400
        1		
        0.199854		
        ${INPUTDATA}/atmosphericCorrectionSequencement_wavelenght_spectral_bands_spot4_1.txt
# ReflectanceToSurfaceReflectance parameters                
# SurfaceAdjencyEffect6SCorrectionSchemeFilter parameters                
	2       # Radius;	
	0.020   # pixel spacing in kilometers
        )


# -------            EVI RAndBAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvEVIRAndBAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS4}  
 --compare-image ${EPSILON}   ${BASELINE}/raRAndBAndNIRVegetationIndex_EVI_poupees_subc1c2c3.tif
                   ${TEMP}/raRAndBAndNIRVegetationIndex_EVI_poupees_subc1c2c3.tif
        otbEVIRAndBAndNIRVegetationIndexImageFilter
        EVI
        ${INPUTDATA}/poupees_sub_c1.png
        ${INPUTDATA}/poupees_sub_c2.png
        ${INPUTDATA}/poupees_sub_c3.png
        ${TEMP}/raRAndBAndNIRVegetationIndex_EVI_poupees_subc1c2c3.tif
        2.5
        6.0
        7.5
        1.0
)

# -------            EVI MultiChannelRAndBAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvEVIMultiChannelRAndBAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS4}  
 --compare-image ${EPSILON}   ${BASELINE}/raRAndBAndNIRVegetationIndex_EVI_qb_RoadExtract.tif
                   ${TEMP}/raRAndBAndNIRVegetationIndex_EVI_qb_RoadExtract.tif
        otbEVIMultiChannelRAndBAndNIRVegetationIndexImageFilter
        EVI
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        ${TEMP}/raRAndBAndNIRVegetationIndex_EVI_qb_RoadExtract.tif
        3   # red
        1   # blue
        4   # nir
        10.0    # gain factor
        30.0    # coefficient of the aerosol resistance term
        7.5     # coefficient of the aerosol resistance term
        400.0   # canopy  background adjustment
)

# -------            TSARVI RAndBAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvTSARVIRAndBAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS4}  
 --compare-image ${EPSILON}   ${BASELINE}/raRAndBAndNIRVegetationIndex_TSARVI_poupees_subc1c2c3.tif
                   ${TEMP}/raRAndBAndNIRVegetationIndex_TSARVI_poupees_subc1c2c3.tif
        otbTSARVIRAndBAndNIRVegetationIndexImageFilter
        ${INPUTDATA}/poupees_sub_c1.png
        ${INPUTDATA}/poupees_sub_c2.png
        ${INPUTDATA}/poupees_sub_c3.png
        ${TEMP}/raRAndBAndNIRVegetationIndex_TSARVI_poupees_subc1c2c3.tif
        0.7   # a ( pente de la droite des sols nus dans l'espace RB/PIR )
        0.9   # b ( ordonnée à l'origine de la droite des sols nus dans l'espace RB/PIR ) 
        0.08  # x coeff a priori constant
        0.5   # gamma
)

# -------            TSARVI MultiChannelRAndBAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvTSARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS4}  
 --compare-image ${EPSILON}   ${BASELINE}/raRAndBAndNIRVegetationIndex_TSARVI_qb_RoadExtract.tif
                   ${TEMP}/raRAndBAndNIRVegetationIndex_TSARVI_qb_RoadExtract.tif
        otbTSARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        ${TEMP}/raRAndBAndNIRVegetationIndex_TSARVI_qb_RoadExtract.tif
        3   # red
        1   # blue
        4   # nir
        0.7   # a ( pente de la droite des sols nus dans l'espace RB/PIR )
        0.9   # b ( ordonnée à l'origine de la droite des sols nus dans l'espace RB/PIR ) 
        0.08  # x coeff a priori constant
        0.5   # gamma
)

# -------            GEMI RAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvGEMIRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS4}  
 --compare-image ${EPSILON}   ${BASELINE}/raRAndNIRVegetationIndex_GEMI_poupees_subc1c2c3.tif
                   ${TEMP}/raRAndNIRVegetationIndex_GEMI_poupees_subc1c2c3.tif
        otbRAndNIRVegetationIndexImageFilter
        GEMI
        ${INPUTDATA}/poupees_sub_c1.png
        ${INPUTDATA}/poupees_sub_c2.png
        ${TEMP}/raRAndNIRVegetationIndex_GEMI_poupees_subc1c2c3.tif
)

# -------            GEMI MultiChannelRAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvGEMIMultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS4}  
 --compare-image ${EPSILON}   ${BASELINE}/raRAndNIRVegetationIndex_GEMI_qb_RoadExtract.tif
                   ${TEMP}/raRAndNIRVegetationIndex_GEMI_qb_RoadExtract.tif
        otbMultiChannelRAndNIRVegetationIndexImageFilter
        GEMI
        ${INPUTDATA}/qb_RoadExtract.img
        ${TEMP}/raRAndNIRVegetationIndex_GEMI_qb_RoadExtract.tif
        3  4   # red nir
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADIOMETRY_TESTS5 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::RAndGAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTuRAndGAndNIRVegetationIndexImageFilterNew ${RADIOMETRY_TESTS5}  
        otbRAndGAndNIRVegetationIndexImageFilterNew )

# -------            otb::MultiChannelRAndGAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTuMultiChannelRAndGAndNIRVegetationIndexImageFilterNew ${RADIOMETRY_TESTS5}  
        otbMultiChannelRAndGAndNIRVegetationIndexImageFilterNew )

# -------            AVI RAndGAndNIRVegetationIndexImageFilter   ------------------------------
# ADD_TEST(raTvAVIRAndGAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}  
#  --compare-image ${EPSILON}   ${BASELINE}/raRAndGAndNIRVegetationIndex_AVI_poupees_subc1c2c3.tif
#                    ${TEMP}/raRAndGAndNIRVegetationIndex_AVI_poupees_subc1c2c3.tif
#         otbAVIRAndGAndNIRVegetationIndexImageFilter
#         ${INPUTDATA}/poupees_sub_c1.png
#         ${INPUTDATA}/poupees_sub_c2.png
#         ${INPUTDATA}/poupees_sub_c3.png
#         ${TEMP}/raRAndGAndNIRVegetationIndex_AVI_poupees_subc1c2c3.tif
#         0.7   # lambda1
#         0.9   # lambda2
#         0.8  # lambda3
# )
# 
# # -------            AVI MultiChannelRAndGAndNIRVegetationIndexImageFilter   ------------------------------
# ADD_TEST(raTvAVIMultiChannelRAndGAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}  
#  --compare-image ${EPSILON}   ${BASELINE}/raRAndGAndNIRVegetationIndex_AVI_qb_RoadExtract.tif
#                    ${TEMP}/raRAndGAndNIRVegetationIndex_AVI_qb_RoadExtract.tif
#         otbAVIMultiChannelRAndGAndNIRVegetationIndexImageFilter
#         ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
#         ${TEMP}/raRAndGAndNIRVegetationIndex_AVI_qb_RoadExtract.tif
#         3   # red
#         2   # green
#         4   # nir
#         0.7   # lambda1
#         0.9   # lambda2
#         0.8  # lambda3
# )
# 
# # -------            WDVI RAndNIRVegetationIndexImageFilter   ------------------------------
# ADD_TEST(raTvWDVIRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}  
#  --compare-image ${EPSILON}   ${BASELINE}/raRAndNIRVegetationIndex_WDVI_poupees_subc1c2c3.tif
#                    ${TEMP}/raRAndNIRVegetationIndex_WDVI_poupees_subc1c2c3.tif
#         otbWDVIRAndNIRVegetationIndexImageFilter
#         ${INPUTDATA}/poupees_sub_c1.png
#         ${INPUTDATA}/poupees_sub_c2.png
#         ${TEMP}/raRAndNIRVegetationIndex_WDVI_poupees_subc1c2c3.tif
#         1.0   # g : slope of soil line
# )
# 
# # -------            WDVI MultiChannelRAndNIRVegetationIndexImageFilter   ------------------------------
# ADD_TEST(raTvWDVIMultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}  
#  --compare-image ${EPSILON}   ${BASELINE}/raRAndNIRVegetationIndex_WDVI_qb_RoadExtract.tif
#                    ${TEMP}/raRAndNIRVegetationIndex_WDVI_qb_RoadExtract.tif
#         otbWDVIMultiChannelRAndNIRVegetationIndexImageFilter
#         ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
#         ${TEMP}/raRAndNIRVegetationIndex_WDVI_qb_RoadExtract.tif
#         3   # red
#         4   # nir
#         1.0   # g : slope of soil line
# )
# 
# # -------            IPVI RAndNIRVegetationIndexImageFilter   ------------------------------
# ADD_TEST(raTvIPVIRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}  
#  --compare-image ${EPSILON}   ${BASELINE}/raRAndNIRVegetationIndex_IPVI_poupees_subc1c2c3.tif
#                    ${TEMP}/raRAndNIRVegetationIndex_IPVI_poupees_subc1c2c3.tif
#         otbRAndNIRVegetationIndexImageFilter
#         ${INPUTDATA}/poupees_sub_c1.png
#         ${INPUTDATA}/poupees_sub_c2.png
#         ${TEMP}/raRAndNIRVegetationIndex_IPVI_poupees_subc1c2c3.tif
# )
# 
# # -------            IPVI MultiChannelRAndNIRVegetationIndexImageFilter   ------------------------------
# ADD_TEST(raTvIPVIMultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}  
#  --compare-image ${EPSILON}   ${BASELINE}/raRAndNIRVegetationIndex_IPVI_qb_RoadExtract.tif
#                    ${TEMP}/raRAndNIRVegetationIndex_IPVI_qb_RoadExtract.tif
#         otbMultiChannelRAndNIRVegetationIndexImageFilter
#         ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
#         ${TEMP}/raRAndNIRVegetationIndex_IPVI_qb_RoadExtract.tif
#         3   # red
#         1   # blue
#         4   # nir
# )
# 
# # -------            TNDVI RAndNIRVegetationIndexImageFilter   ------------------------------
# ADD_TEST(raTvTNDVIRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}  
#  --compare-image ${EPSILON}   ${BASELINE}/raRAndNIRVegetationIndex_TNDVI_poupees_subc1c2c3.tif
#                    ${TEMP}/raRAndNIRVegetationIndex_TNDVI_poupees_subc1c2c3.tif
#         otbRAndNIRVegetationIndexImageFilter
#         ${INPUTDATA}/poupees_sub_c1.png
#         ${INPUTDATA}/poupees_sub_c2.png
#         ${TEMP}/raRAndNIRVegetationIndex_TNDVI_poupees_subc1c2c3.tif
# )
# 
# # -------            TNDVI MultiChannelRAndNIRVegetationIndexImageFilter   ------------------------------
# ADD_TEST(raTvTNDVIMultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}  
#  --compare-image ${EPSILON}   ${BASELINE}/raRAndNIRVegetationIndex_TNDVI_qb_RoadExtract.tif
#                    ${TEMP}/raRAndNIRVegetationIndex_TNDVI_qb_RoadExtract.tif
#         otbMultiChannelRAndNIRVegetationIndexImageFilter
#         ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
#         ${TEMP}/raRAndNIRVegetationIndex_TNDVI_qb_RoadExtract.tif
#         3   # red
#         1   # blue
#         4   # nir
# )


# A enrichir
SET(Radiometry_SRCS1
otbRAndNIRVegetationIndexImageFilterNew.cxx
otbRAndNIRVegetationIndexImageFilter.cxx
otbSetASetBRAndNIRVegetationIndexImageFilter.cxx
otbMultiChannelRAndNIRVegetationIndexImageFilterNew.cxx
otbMultiChannelRAndNIRVegetationIndexImageFilter.cxx
otbSetASetBMultiChannelRAndNIRVegetationIndexImageFilter.cxx
)
SET(Radiometry_SRCS2
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
)
SET(Radiometry_SRCS3
otbDEMCaracteristicsExtractorNew.cxx
otbDEMCaracteristicsExtractor.cxx
otbAtmosphericCorrectionParametersNew.cxx
otbFilterFunctionValues.cxx
otbSIXSTraitsComputeAtmosphericParameters.cxx
otbAtmosphericRadiativeTermsNew.cxx
otbReflectanceToSurfaceReflectanceImageFilterNew.cxx
otbReflectanceToSurfaceReflectanceImageFilter.cxx
otbSurfaceAdjencyEffect6SCorrectionSchemeFilterNew.cxx
otbSurfaceAdjencyEffect6SCorrectionSchemeFilter.cxx
otbRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter.cxx
)
SET(Radiometry_SRCS4
otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTermsNew.cxx
otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms.cxx
otbAtmosphericCorrectionSequencement.cxx
otbEVIRAndBAndNIRVegetationIndexImageFilter.cxx
otbEVIMultiChannelRAndBAndNIRVegetationIndexImageFilter.cxx
otbTSARVIRAndBAndNIRVegetationIndexImageFilter.cxx
otbTSARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter.cxx

# je remets ça ? DUPLICATION pas bon
otbRAndNIRVegetationIndexImageFilter.cxx
otbMultiChannelRAndNIRVegetationIndexImageFilter.cxx
)
SET(Radiometry_SRCS5
otbRAndGAndNIRVegetationIndexImageFilterNew.cxx
otbMultiChannelRAndGAndNIRVegetationIndexImageFilterNew.cxx
otbAVIRAndGAndNIRVegetationIndexImageFilter.cxx
otbAVIMultiChannelRAndGAndNIRVegetationIndexImageFilter.cxx
otbWDVIRAndNIRVegetationIndexImageFilter.cxx
otbWDVIMultiChannelRAndNIRVegetationIndexImageFilter.cxx


# je remets ça ? DUPLICATION pas bon
otbRAndNIRVegetationIndexImageFilter.cxx
otbMultiChannelRAndNIRVegetationIndexImageFilter.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbRadiometryTests1 otbRadiometryTests1.cxx ${Radiometry_SRCS1})
TARGET_LINK_LIBRARIES(otbRadiometryTests1 OTBRadiometry OTBIO)
ADD_EXECUTABLE(otbRadiometryTests2 otbRadiometryTests2.cxx ${Radiometry_SRCS2})
TARGET_LINK_LIBRARIES(otbRadiometryTests2 OTBRadiometry OTBIO)
ADD_EXECUTABLE(otbRadiometryTests3 otbRadiometryTests3.cxx ${Radiometry_SRCS3})
TARGET_LINK_LIBRARIES(otbRadiometryTests3 OTBRadiometry OTBIO)
ADD_EXECUTABLE(otbRadiometryTests4 otbRadiometryTests4.cxx ${Radiometry_SRCS4})
TARGET_LINK_LIBRARIES(otbRadiometryTests4 OTBRadiometry OTBIO)
ADD_EXECUTABLE(otbRadiometryTests5 otbRadiometryTests5.cxx ${Radiometry_SRCS5})
TARGET_LINK_LIBRARIES(otbRadiometryTests5 OTBRadiometry OTBIO)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
