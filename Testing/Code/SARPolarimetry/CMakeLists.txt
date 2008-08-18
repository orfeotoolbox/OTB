
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
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
ADD_TEST(rdTvPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${TOL}   ${BASELINE}/rdTvPolarimetricSynthesis.tif
                    ${TEMP}/resPolarimetricSynthesis.tif
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
ADD_TEST(rdTvHHnHVnVVPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${TOL}   ${BASELINE}/rdTvPolarimetricSynthesis_HH_HV_VV.tif
                    ${TEMP}/resPolarimetricSynthesis_HH_HV_VV.tif
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
ADD_TEST(rdTvHHnHVPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${TOL}   ${BASELINE}/rdTvPolarimetricSynthesis_HH_HV.tif
                    ${TEMP}/resPolarimetricSynthesis_HH_HV.tif
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
ADD_TEST(rdTvHHnVVPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
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
     
 # With HH, HV, VH and VV channels      
ADD_TEST(rdTvMultiChannelsPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}  
  --compare-image ${TOL}   ${BASELINE}/rdTvMultiPolarimetricSynthesis.tif
                    ${TEMP}/resMultiPolarimetricSynthesis.tif
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
        
SET_TESTS_PROPERTIES(rdTvHHnVVPolarimetricSynthesisFilter PROPERTIES WILL_FAIL true)
        
# A enrichir
SET(SARPOLARIMETRY_SRCS1
otbPolarimetricSynthesisFilterNew.cxx
otbPolarimetricSynthesisFilter.cxx
otbHHnHVnVVPolarimetricSynthesisFilter.cxx
otbHHnHVPolarimetricSynthesisFilter.cxx
otbHHnVVPolarimetricSynthesisFilter.cxx
otbMultiChannelsPolarimetricSynthesisFilterNew.cxx
otbMultiChannelsPolarimetricSynthesisFilter.cxx
)



INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbSARPolarimetryTests1 otbSARPolarimetryTests1.cxx ${SARPOLARIMETRY_SRCS1})
TARGET_LINK_LIBRARIES(otbSARPolarimetryTests1 OTBSARPolarimetry OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
