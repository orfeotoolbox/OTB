
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


# SinclairToCovarianceFunctor
ADD_TEST(sarTuSinclairToCovarianceFunctor ${SARPOLARIMETRY_TESTS1}
		otbSinclairToCovarianceFunctor
)

# Sinclair To Covariance Image Filter
ADD_TEST(sarTvSinclairImageFilter_SinclairToCovariance ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/sarTvSinclairImageFilter_SinclairToCovariance.tif
                    ${TEMP}/sarTvSinclairImageFilter_SinclairToCovariance.tif
        otbSinclairImageFilter
        SinclairToCovariance
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/sarTvSinclairImageFilter_SinclairToCovariance.tif
	)


# Sinclair To Coherency Image Filter
ADD_TEST(sarTvSinclairImageFilter_SinclairToCoherency ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/sarTvSinclairImageFilter_SinclairToCoherency.tif
                    ${TEMP}/sarTvSinclairImageFilter_SinclairToCoherency.tif
        otbSinclairImageFilter
        SinclairToCoherency
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/sarTvSinclairImageFilter_SinclairToCoherency.tif
	)

# Sinclair To Mueller Image Filter
ADD_TEST(sarTvSinclairImageFilter_SinclairToMueller ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/sarTvSinclairImageFilter_SinclairToMueller.tif
                    ${TEMP}/sarTvSinclairImageFilter_SinclairToMueller.tif
        otbSinclairImageFilter
        SinclairToMueller
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/sarTvSinclairImageFilter_SinclairToMueller.tif
	)

# MLC To Coherency Image Filter
ADD_TEST(sarTuMLCToCoherencyImageFilterNew ${SARPOLARIMETRY_TESTS1}
		otbMLCToCoherencyImageFilterNew
)

ADD_TEST(sarTvMLCToCoherencyImageFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/sarTvMLCToCoherencyImageFilter.tif
                    ${TEMP}/sarTvMLCToCoherencyImageFilter.tif
        otbMLCToCoherencyImageFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/sarTvMLCToCoherencyImageFilter.tif
	)

# MLC To Coherency Degree Image Filter
ADD_TEST(sarTuMLCToCoherencyDegreeImageFilterNew ${SARPOLARIMETRY_TESTS1}
		otbMLCToCoherencyDegreeImageFilterNew
)

ADD_TEST(sarTvMLCToCoherencyDegreeImageFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/sarTvMLCToCoherencyDegreeImageFilter.tif
                    ${TEMP}/sarTvMLCToCoherencyDegreeImageFilter.tif
        otbMLCToCoherencyDegreeImageFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/sarTvMLCToCoherencyDegreeImageFilter.tif
	)

# MLC To Circular Coherency Degree Image Filter
ADD_TEST(sarTuMLCToCircularCoherencyDegreeImageFilterNew ${SARPOLARIMETRY_TESTS1}
		otbMLCToCircularCoherencyDegreeImageFilterNew
)

ADD_TEST(sarTvMLCToCircularCoherencyDegreeImageFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/sarTvMLCToCircularCoherencyDegreeImageFilter.tif
                    ${TEMP}/sarTvMLCToCircularCoherencyDegreeImageFilter.tif
        otbMLCToCircularCoherencyDegreeImageFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/sarTvMLCToCircularCoherencyDegreeImageFilter.tif
	)

# H Alpha Image Filter
ADD_TEST(sarTuHAlphaImageFilterNew ${SARPOLARIMETRY_TESTS1}
		otbHAlphaImageFilterNew
)

ADD_TEST(sarTvHAlphaImageFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/sarTvHAlphaImageFilter.tif
                    ${TEMP}/sarTvHAlphaImageFilter.tif
        otbHAlphaImageFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/sarTvHAlphaImageFilter.tif
	)

# Coherency To Mueller Image Filter
ADD_TEST(sarTuCoherencyToMuellerImageFilterNew ${SARPOLARIMETRY_TESTS1}
		otbCoherencyToMuellerImageFilterNew
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
otbSinclairToCovarianceFunctor.cxx
otbSinclairImageFilter.cxx
otbMLCToCoherencyImageFilterNew.cxx
otbMLCToCoherencyImageFilter.cxx
otbMLCToCoherencyDegreeImageFilterNew.cxx
otbMLCToCoherencyDegreeImageFilter.cxx
otbMLCToCircularCoherencyDegreeImageFilterNew.cxx
otbMLCToCircularCoherencyDegreeImageFilter.cxx
otbHAlphaImageFilterNew.cxx
otbHAlphaImageFilter.cxx
otbCoherencyToMuellerImageFilterNew.cxx
)

OTB_ADD_EXECUTABLE(otbSARPolarimetryTests1 "${SARPOLARIMETRY_SRCS1}" "OTBSARPolarimetry;OTBIO;OTBTesting")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
