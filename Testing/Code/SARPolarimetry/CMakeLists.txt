
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
                    ${TEMP}/resPolarimetricSynthesis_HH_VH_HV_VV.tif
        otbPolarimetricSynthesisFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VH.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif                        
        ${TEMP}/resPolarimetricSynthesis_HH_VH_HV_VV.tif
         32.0     # PsiI
         22.0     # KhiI
         58.0     # PsiR
         45.0     # KhiR
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
        
# With VH and VV channels
ADD_TEST(rdTvVHnVVPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
#  --compare-image ${TOL}   ${BASELINE}/rdTvPolarimetricSynthesis_VH_VV.tif
#                    ${TEMP}/resPolarimetricSynthesis_VH_VV.tif
        otbHHnHVPolarimetricSynthesisFilter
        ${INPUTDATA}/RSAT_imagery_VH.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/resPolarimetricSynthesis_VH_VV.tif
         32.0
         22.0
         58.0
         45.0
	)             
        
ADD_TEST(rdTuMultiChannelsPolarimetricSynthesisFilterNew ${SARPOLARIMETRY_TESTS1}  
        otbMultiChannelsPolarimetricSynthesisFilterNew
	)
            
# With vector image HH, HV, VH and VV channels      
#ADD_TEST(rdTvVectorMultiChannelsPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
#  --compare-image ${TOL}   ${BASELINE}/rdTvMultiPolarimetricSynthesis1.tif
#                    ${TEMP}/resMultiPolarimetricSynthesis1.tif
#        otbVectorMultiChannelsPolarimetricSynthesisFilter
#        ${IMAGEDATA}/RSAT2/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/product.xml
#        ${TEMP}/resMultiPolarimetricSynthesis1.tif
#         0.0     # PsiI
#         0.0     # KhiI
#         0.0     # PsiR
#         0.0     # KhiR
#	)
        
        
# With 3 images HH, HV and VV channels turned into vectorImage      
ADD_TEST(rdTvMultiChannelsPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}  
  --compare-image ${TOL}   ${BASELINE}/rdTvMultiPolarimetricSynthesis2.tif
                    ${TEMP}/resMultiPolarimetricSynthesis2.tif
        otbMultiChannelsPolarimetricSynthesisFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif        
        ${TEMP}/resMultiPolarimetricSynthesis2.tif
         32.0     # PsiI
         22.0     # KhiI
         58.0     # PsiR
         45.0     # KhiR
	)        
        
        
# A enrichir
SET(SARPOLARIMETRY_SRCS1
otbPolarimetricSynthesisFilterNew.cxx
otbPolarimetricSynthesisFilter.cxx
otbHHnHVnVVPolarimetricSynthesisFilter.cxx
otbHHnHVPolarimetricSynthesisFilter.cxx
otbVHnVVPolarimetricSynthesisFilter.cxx
otbMultiChannelsPolarimetricSynthesisFilterNew.cxx
otbMultiChannelsPolarimetricSynthesisFilter.cxx
otbVectorMultiChannelsPolarimetricSynthesisFilter.cxx
)



INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbSARPolarimetryTests1 otbSARPolarimetryTests1.cxx ${SARPOLARIMETRY_SRCS1})
TARGET_LINK_LIBRARIES(otbSARPolarimetryTests1 OTBSARPolarimetry OTBIO)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
