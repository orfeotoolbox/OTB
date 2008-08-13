
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.000000000001)
SET(EPSILON_10e6 0.000001)
SET(SARPOLARIMETRY_TESTS1 ${CXX_TEST_PATH}/otbSARPolarimetryTests1)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbSARPolarimetry_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


ADD_TEST(rdTuPolarimetricSynthesisFilterNew ${SARPOLARIMETRY_TESTS1}  
        otbPolarimetricSynthesisFilterNew )

 # Normal Case 
ADD_TEST(rdTuPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
 # --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
 #                   ${TEMP}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
        otbPolarimetricSynthesisFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VH.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif                        
        ${TEMP}/resPolarimetricSynthesis.tif
         32.0     # PsiI
         22.0     # TauI
         58.0     # PsiR
         45.0     # TauR
	)
        
 # With 3 channels
ADD_TEST(rdTuHHnHVnVVPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
 # --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
 #                   ${TEMP}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
        otbHHnHVnVVPolarimetricSynthesisFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif                        
        ${TEMP}/resPolarimetricSynthesis_HH_HV_VV.tif
         32.0
         22.0
         58.0
         45.0
	)

 # With HH and HV channels
ADD_TEST(rdTuHHnHVPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
 # --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
 #                   ${TEMP}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
        otbHHnHVPolarimetricSynthesisFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${TEMP}/resPolarimetricSynthesis_HH_HV.tif
         32.0
         22.0
         58.0
         45.0
	)
        
 # With HH and VV channels        
ADD_TEST(rdTuHHnVVPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
 # --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
 #                   ${TEMP}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
        otbHHnVVPolarimetricSynthesisFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/resPolarimetricSynthesis_HH_VV.tif
         32.0
         22.0
         58.0
         45.0
	)        
        
ADD_TEST(rdTuMultiChannelsPolarimetricSynthesisFilterNew ${SARPOLARIMETRY_TESTS1}  
        otbMultiChannelsPolarimetricSynthesisFilterNew
	)
     
ADD_TEST(rdTuMultiChannelsPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}  
        otbMultiChannelsPolarimetricSynthesisFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VH.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif        
        ${TEMP}/resMultiPolarimetricSynthesis.tif
         32.0     # PsiI
         22.0     # TauI
         58.0     # PsiR
         45.0     # TauR
	)        
        
SET_TESTS_PROPERTIES(rdTuHHnVVPolarimetricSynthesisFilter PROPERTIES WILL_FAIL true)
        
# A enrichir
SET(SARPOLARIMETRY_SRCS1
otbPolarimetricSynthesisFilterNew.cxx
otbPolarimetricSynthesisFilter.cxx
otbHHnHVnVVPolarimetricSynthesisFilter
otbHHnHVPolarimetricSynthesisFilter
otbHHnVVPolarimetricSynthesisFilter
otbMultiChannelsPolarimetricSynthesisFilterNew.cxx
otbMultiChannelsPolarimetricSynthesisFilter.cxx
otbMultiChannelsPolarimetricSynthesisFilter.cxx
)




INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbSARPolarimetryTests1 otbSARPolarimetryTests1.cxx ${SARPOLARIMETRY_SRCS1})
TARGET_LINK_LIBRARIES(otbSARPolarimetryTests1 OTBSARPolarimetry OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
