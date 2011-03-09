
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
SET(SARPOLARIMETRY_TESTS2 ${CXX_TEST_PATH}/otbSARPolarimetryTests2)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbSARPolarimetry_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


ADD_TEST(saTuPolarimetricSynthesisFilterNew ${SARPOLARIMETRY_TESTS1}
        otbPolarimetricSynthesisFilterNew )

 # Normal Case
ADD_TEST(saTvPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvPolarimetricSynthesis.tif
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
ADD_TEST(saTvHHnHVnVVPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvPolarimetricSynthesis_HH_HV_VV.tif
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
ADD_TEST(saTvHHnHVPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvPolarimetricSynthesis_HH_HV.tif
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
ADD_TEST(saTvVHnVVPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvPolarimetricSynthesis_VH_VV.tif
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

ADD_TEST(saTuMultiChannelsPolarimetricSynthesisFilterNew ${SARPOLARIMETRY_TESTS1}
        otbMultiChannelsPolarimetricSynthesisFilterNew
	)

# With vector image HH, HV, VH and VV channels
IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(saTvVectorMultiChannelsPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
 --compare-image ${EPSILON_12}   ${BASELINE}/saTvMultiPolarimetricSynthesis1.tif
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
ADD_TEST(saTvMultiChannelsPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMultiPolarimetricSynthesis2.tif
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

# otbSinclairToCircularCovarianceMatrixFunctor
ADD_TEST(saTuSinclairToCircularCovarianceMatrixFunctor ${SARPOLARIMETRY_TESTS1}
		otbSinclairToCircularCovarianceMatrixFunctor
)

# otbSinclairToMuellerFunctor
ADD_TEST(saTuSinclairToMuellerFunctor ${SARPOLARIMETRY_TESTS1}
		otbSinclairToMuellerFunctor
)

# otbSinclairToReciprocalCovarianceFunctor
ADD_TEST(saTuSinclairToReciprocalCovarianceFunctor ${SARPOLARIMETRY_TESTS1}
		otbSinclairToReciprocalCovarianceFunctor
)

# otbSinclairToCoherencyFunctor
ADD_TEST(saTuSinclairToCoherencyFunctor ${SARPOLARIMETRY_TESTS1}
		otbSinclairToCoherencyFunctor
)

# otbSinclairToReciprocalCircularCovarianceMatrixFunctor
ADD_TEST(saTuSinclairToReciprocalCircularCovarianceMatrixFunctor ${SARPOLARIMETRY_TESTS1}
		otbSinclairToReciprocalCircularCovarianceMatrixFunctor
)

# otbSinclairToCovarianceFunctor
ADD_TEST(saTuSinclairToCovarianceFunctor ${SARPOLARIMETRY_TESTS1}
		otbSinclairToCovarianceFunctor
)

# otbSinclairToReciprocalCoherencyFunctor
ADD_TEST(saTuSinclairToReciprocalCoherencyFunctor ${SARPOLARIMETRY_TESTS1}
		otbSinclairToReciprocalCoherencyFunctor
)

ADD_TEST(saTuSinclairImageFilterNew ${SARPOLARIMETRY_TESTS1}
		otbSinclairImageFilterNew
)

# Sinclair To Covariance Image Filter
ADD_TEST(saTvSinclairImageFilter_SinclairToCovariance ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCovariance.tif
                    ${TEMP}/saTvSinclairImageFilter_SinclairToCovariance.tif
        otbSinclairImageFilter
        SinclairToCovariance
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvSinclairImageFilter_SinclairToCovariance.tif
	)

# Sinclair To Circular Covariance Image Filter
ADD_TEST(saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix.tif
                    ${TEMP}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix.tif
        otbSinclairImageFilter
        SinclairToCircularCovarianceMatrix
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix.tif
	)

# Sinclair To Coherency Image Filter
ADD_TEST(saTvSinclairImageFilter_SinclairToCoherency ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCoherency.tif
                    ${TEMP}/saTvSinclairImageFilter_SinclairToCoherency.tif
        otbSinclairImageFilter
        SinclairToCoherency
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvSinclairImageFilter_SinclairToCoherency.tif
	)

# Sinclair To Mueller Image Filter
ADD_TEST(saTvSinclairImageFilter_SinclairToMueller ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
                    ${TEMP}/saTvSinclairImageFilter_SinclairToMueller.tif
        otbSinclairImageFilter
        SinclairToMueller
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvSinclairImageFilter_SinclairToMueller.tif
	)


ADD_TEST(saTuSinclairReciprocalImageFilterNew ${SARPOLARIMETRY_TESTS1}
		otbSinclairReciprocalImageFilterNew
)

# Sinclair To Reciprocal Covariance Image Filter
ADD_TEST(saTvSinclairReciprocalImageFilter_SinclairToReciprocalCovariance ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
                    ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
        otbSinclairReciprocalImageFilter
        SinclairToReciprocalCovariance
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
	)

# Sinclair To Reciprocal Circular Covariance Image Filter
ADD_TEST(saTvSinclairReciprocalImageFilter_SinclairToReciprocalCircularCovarianceMatrix ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
                    ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
        otbSinclairReciprocalImageFilter
        SinclairToReciprocalCircularCovarianceMatrix
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
	)

# Sinclair To Reciprocal Coherency Image Filter
ADD_TEST(saTvSinclairReciprocalImageFilter_SinclairToReciprocalCoherency ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
                    ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
        otbSinclairReciprocalImageFilter
        SinclairToReciprocalCoherency
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
	)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbSARPolarimetry_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# MLC To Coherency Image Filter
ADD_TEST(saTuMLCToCoherencyImageFilterNew ${SARPOLARIMETRY_TESTS2}
		otbMLCToCoherencyImageFilterNew
)

ADD_TEST(saTvMLCToCoherencyImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMLCToCoherencyImageFilter.tif
                    ${TEMP}/saTvMLCToCoherencyImageFilter.tif
        otbMLCToCoherencyImageFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvMLCToCoherencyImageFilter.tif
	)

# MLC To Coherency Degree Image Filter
ADD_TEST(saTuMLCToCoherencyDegreeImageFilterNew ${SARPOLARIMETRY_TESTS2}
		otbMLCToCoherencyDegreeImageFilterNew
)

ADD_TEST(saTvMLCToCoherencyDegreeImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMLCToCoherencyDegreeImageFilter.tif
                    ${TEMP}/saTvMLCToCoherencyDegreeImageFilter.tif
        otbMLCToCoherencyDegreeImageFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvMLCToCoherencyDegreeImageFilter.tif
	)

# MLC To Circular Coherency Degree Image Filter
ADD_TEST(saTuMLCToCircularCoherencyDegreeImageFilterNew ${SARPOLARIMETRY_TESTS2}
		otbMLCToCircularCoherencyDegreeImageFilterNew
)

ADD_TEST(saTvMLCToCircularCoherencyDegreeImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMLCToCircularCoherencyDegreeImageFilter.tif
                    ${TEMP}/saTvMLCToCircularCoherencyDegreeImageFilter.tif
        otbMLCToCircularCoherencyDegreeImageFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvMLCToCircularCoherencyDegreeImageFilter.tif
	)

# H Alpha Image Filter
ADD_TEST(saTuReciprocalHAlphaImageFilterNew ${SARPOLARIMETRY_TESTS2}
		otbReciprocalHAlphaImageFilterNew
)

ADD_TEST(saTvReciprocalHAlphaImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvReciprocalHAlphaImageFilter.tif
                    ${TEMP}/saTvReciprocalHAlphaImageFilter.tif
        otbReciprocalHAlphaImageFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvReciprocalHAlphaImageFilter.tif
	)

# Reciprocal Coherency To Mueller Image Filter
ADD_TEST(saTuReciprocalCoherencyToMuellerImageFilterNew ${SARPOLARIMETRY_TESTS2}
		otbReciprocalCoherencyToMuellerImageFilterNew
)

ADD_TEST(saTvReciprocalCoherencyToMuellerImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvReciprocalCoherencyToMuellerImageFilter.tif
                    ${TEMP}/saTvReciprocalCoherencyToMuellerImageFilter.tif
        otbReciprocalCoherencyToMuellerImageFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvReciprocalCoherencyToMuellerImageFilter.tif
	)

# Mueller To Circular polarisation Image Filter
ADD_TEST(saTuMuellerToCircularPolarisationImageFilterNew ${SARPOLARIMETRY_TESTS2}
		otbMuellerToCircularPolarisationImageFilterNew
)

ADD_TEST(saTvMuellerToCircularPolarisationImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMuellerToCircularPolarisationImageFilter.tif
                    ${TEMP}/saTvMuellerToCircularPolarisationImageFilter.tif
        otbMuellerToCircularPolarisationImageFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvMuellerToCircularPolarisationImageFilter.tif
	)

# Mueller To polarisation degree and power Image Filter
ADD_TEST(saTuMuellerToPolarisationDegreeAndPowerImageFilterNew ${SARPOLARIMETRY_TESTS2}
		otbMuellerToPolarisationDegreeAndPowerImageFilterNew
)

ADD_TEST(saTvMuellerToPolarisationDegreeAndPowerImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
                    ${TEMP}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
        otbMuellerToPolarisationDegreeAndPowerImageFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
	)

# Mueller To MLC Image Filter
ADD_TEST(saTuMuellerToMLCImageFilterNew ${SARPOLARIMETRY_TESTS2}
		otbMuellerToMLCImageFilterNew
)

ADD_TEST(saTvMuellerToMLCImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMuellerToMLCImageFilter.tif
                    ${TEMP}/saTvMuellerToMLCImageFilter.tif
        otbMuellerToMLCImageFilter
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvMuellerToMLCImageFilter.tif
	)
# Hermitian eigen analysis class
ADD_TEST(saTvHermitianEigenAnalysisTest ${SARPOLARIMETRY_TESTS2}
		otbHermitianEigenAnalysisTest
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
otbSinclairToCircularCovarianceMatrixFunctor.cxx
otbSinclairToMuellerFunctor.cxx
otbSinclairToReciprocalCovarianceFunctor.cxx
otbSinclairToCoherencyFunctor.cxx
otbSinclairToReciprocalCircularCovarianceMatrixFunctor.cxx
otbSinclairToCovarianceFunctor.cxx
otbSinclairToReciprocalCoherencyFunctor.cxx
otbSinclairImageFilter.cxx
otbSinclairReciprocalImageFilter.cxx
)


SET(SARPOLARIMETRY_SRCS2
otbSARPolarimetryTests2.cxx
otbMLCToCoherencyImageFilterNew.cxx
otbMLCToCoherencyImageFilter.cxx
otbMLCToCoherencyDegreeImageFilterNew.cxx
otbMLCToCoherencyDegreeImageFilter.cxx
otbMLCToCircularCoherencyDegreeImageFilterNew.cxx
otbMLCToCircularCoherencyDegreeImageFilter.cxx
otbReciprocalHAlphaImageFilterNew.cxx
otbReciprocalHAlphaImageFilter.cxx
otbReciprocalCoherencyToMuellerImageFilterNew.cxx
otbReciprocalCoherencyToMuellerImageFilter.cxx
otbMuellerToCircularPolarisationImageFilterNew.cxx
otbMuellerToCircularPolarisationImageFilter.cxx
otbMuellerToPolarisationDegreeAndPowerImageFilterNew.cxx
otbMuellerToPolarisationDegreeAndPowerImageFilter.cxx
otbMuellerToMLCImageFilterNew.cxx
otbMuellerToMLCImageFilter.cxx
otbHermitianEigenAnalysisTest.cxx
)


OTB_ADD_EXECUTABLE(otbSARPolarimetryTests1 "${SARPOLARIMETRY_SRCS1}" "OTBSARPolarimetry;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbSARPolarimetryTests2 "${SARPOLARIMETRY_SRCS2}" "OTBSARPolarimetry;OTBIO;OTBTesting")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
