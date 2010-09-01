
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(IMAGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(NOTOL 0.0)
SET(EPSILON_12 0.000000000001)
SET(EPSILON_6 0.000001)
SET(SARPOLARIMETRY_TESTS1 ${CXX_TEST_PATH}/otbSARPolarimetryTests1)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbSARPolarimetry_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


ADD_TEST(sarTuPolarimetricSynthesisFilterNew ${SARPOLARIMETRY_TESTS1}
        otbPolarimetricSynthesisFilterNew )

 # Normal Case
ADD_TEST(sarTvPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/sarTvPolarimetricSynthesis.tif
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
ADD_TEST(sarTvHHnHVnVVPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/sarTvPolarimetricSynthesis_HH_HV_VV.tif
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
ADD_TEST(sarTvHHnHVPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/sarTvPolarimetricSynthesis_HH_HV.tif
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
ADD_TEST(sarTvVHnVVPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/sarTvPolarimetricSynthesis_VH_VV.tif
                    ${TEMP}/resPolarimetricSynthesis_VH_VV.tif
        otbVHnVVPolarimetricSynthesisFilter
        ${INPUTDATA}/RSAT_imagery_VH.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/resPolarimetricSynthesis_VH_VV.tif
         32.0
         22.0
         58.0
         45.0
	)

ADD_TEST(sarTuMultiChannelsPolarimetricSynthesisFilterNew ${SARPOLARIMETRY_TESTS1}
        otbMultiChannelsPolarimetricSynthesisFilterNew
	)

# With vector image HH, HV, VH and VV channels
IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(sarTvVectorMultiChannelsPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
 --compare-image ${EPSILON_12}   ${BASELINE}/sarTvMultiPolarimetricSynthesis1.tif
                   ${TEMP}/resMultiPolarimetricSynthesis1.tif
       otbVectorMultiChannelsPolarimetricSynthesisFilter
       ${INPUTDATA}/RSAT_imagery_vector.tif.hdr
       ${TEMP}/resMultiPolarimetricSynthesis1.tif
         10.0     # PsiI
         0.0     # KhiI
         0.0     # PsiR
         0.0     # KhiR
	)
ENDIF(OTB_DATA_USE_LARGEINPUT)


# With 3 images HH, HV and VV channels turned into vectorImage
ADD_TEST(sarTvMultiChannelsPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/sarTvMultiPolarimetricSynthesis2.tif
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
otbSARPolarimetryTests1.cxx
otbPolarimetricSynthesisFilterNew.cxx
otbPolarimetricSynthesisFilter.cxx
otbHHnHVnVVPolarimetricSynthesisFilter.cxx
otbHHnHVPolarimetricSynthesisFilter.cxx
otbVHnVVPolarimetricSynthesisFilter.cxx
otbMultiChannelsPolarimetricSynthesisFilterNew.cxx
otbMultiChannelsPolarimetricSynthesisFilter.cxx
otbVectorMultiChannelsPolarimetricSynthesisFilter.cxx
)

OTB_ADD_EXECUTABLE(otbSARPolarimetryTests1 "${SARPOLARIMETRY_SRCS1}" "OTBSARPolarimetry;OTBIO;OTBTesting")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
