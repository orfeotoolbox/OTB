
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(SARPOLARIMETRY_TESTS1 ${CXX_TEST_PATH}/otbSARPolarimetryTests1)
SET(SARPOLARIMETRY_TESTS2 ${CXX_TEST_PATH}/otbSARPolarimetryTests2)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbSARPolarimetry_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADD_TEST(saTuPolarimetricSynthesisFunctor ${SARPOLARIMETRY_TESTS1}
        otbPolarimetricSynthesisFunctor )


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
       ${INPUTDATA}/RSAT2_AltonaExtract_1000_1000_100_100.hdr
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

# otbSinclairToMuellerMatrixFunctor
ADD_TEST(saTuSinclairToMuellerMatrixFunctor ${SARPOLARIMETRY_TESTS1}
                otbSinclairToMuellerMatrixFunctor
)

# otbSinclairToReciprocalCovariancMatrixeFunctor
ADD_TEST(saTuSinclairToReciprocalCovarianceMatrixFunctor ${SARPOLARIMETRY_TESTS1}
                otbSinclairToReciprocalCovarianceMatrixFunctor
)

# otbSinclairToCoherencyFunctor
ADD_TEST(saTuSinclairToCoherencyMatrixFunctor ${SARPOLARIMETRY_TESTS1}
                otbSinclairToCoherencyMatrixFunctor
)

# otbSinclairToReciprocalCircularCovarianceMatrixFunctor
ADD_TEST(saTuSinclairToReciprocalCircularCovarianceMatrixFunctor ${SARPOLARIMETRY_TESTS1}
                otbSinclairToReciprocalCircularCovarianceMatrixFunctor
)

# otbSinclairToCovarianceFunctor
ADD_TEST(saTuSinclairToCovarianceMatrixFunctor ${SARPOLARIMETRY_TESTS1}
                otbSinclairToCovarianceMatrixFunctor
)

# otbSinclairToReciprocalCoherencyFunctor
ADD_TEST(saTuSinclairToReciprocalCoherencyMatrixFunctor ${SARPOLARIMETRY_TESTS1}
                otbSinclairToReciprocalCoherencyMatrixFunctor
)

ADD_TEST(saTuSinclairImageFilterNew ${SARPOLARIMETRY_TESTS1}
                otbSinclairImageFilterNew
)

# Sinclair To Covariance Image Filter
ADD_TEST(saTvSinclairImageFilter_SinclairToCovariance ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCovariance.tif
                    ${TEMP}/saTvSinclairImageFilter_SinclairToCovariance.tif
        otbSinclairImageFilter
        SinclairToCovarianceMatrix
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
        SinclairToCoherencyMatrix
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
        SinclairToMuellerMatrix
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
        SinclairToReciprocalCovarianceMatrix
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
        SinclairToReciprocalCoherencyMatrix
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
        )


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbSARPolarimetry_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# Reciprocal Covariance To Coherency Image Filter
ADD_TEST(saTuReciprocalCovarianceToReciprocalCoherencyImageFilterNew ${SARPOLARIMETRY_TESTS2}
                otbReciprocalCovarianceToReciprocalCoherencyImageFilterNew
)

ADD_TEST(saTvReciprocalCovarianceToReciprocalCoherencyImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMLCToCoherencyImageFilter.tif
                    ${TEMP}/saTvMLCToCoherencyImageFilter.tif
        otbReciprocalCovarianceToReciprocalCoherencyImageFilter
        ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
        ${TEMP}/saTvMLCToCoherencyImageFilter.tif
        )

# Reciprocal Covariance To Coherency Degree Image Filter
ADD_TEST(saTuReciprocalCovarianceToCoherencyDegreeImageFilterNew ${SARPOLARIMETRY_TESTS2}
                otbReciprocalCovarianceToCoherencyDegreeImageFilterNew
)

ADD_TEST(saTvReciprocalCovarianceToCoherencyDegreeImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMLCToCoherencyDegreeImageFilter.tif
                    ${TEMP}/saTvMLCToCoherencyDegreeImageFilter.tif
        otbReciprocalCovarianceToCoherencyDegreeImageFilter
        ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
        ${TEMP}/saTvMLCToCoherencyDegreeImageFilter.tif
        )

# Reciprocal Covariance To Circular Coherency Degree Image Filter
ADD_TEST(saTuReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilterNew ${SARPOLARIMETRY_TESTS2}
                otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilterNew
)

ADD_TEST(saTvReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMLCToCircularCoherencyDegreeImageFilter.tif
                    ${TEMP}/saTvMLCToCircularCoherencyDegreeImageFilter.tif
        otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter
        ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
        ${TEMP}/saTvMLCToCircularCoherencyDegreeImageFilter.tif
        )

# Reciprocal H Alpha Image Filter
ADD_TEST(saTuReciprocalHAlphaImageFilterNew ${SARPOLARIMETRY_TESTS2}
                otbReciprocalHAlphaImageFilterNew
)

ADD_TEST(saTvReciprocalHAlphaImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvReciprocalHAlphaImageFilter.tif
                    ${TEMP}/saTvReciprocalHAlphaImageFilter.tif
        otbReciprocalHAlphaImageFilter
        ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
        ${TEMP}/saTvReciprocalHAlphaImageFilter.tif
        )

# Reciprocal Coherency To Mueller Image Filter
ADD_TEST(saTuReciprocalCoherencyToReciprocalMuellerImageFilterNew ${SARPOLARIMETRY_TESTS2}
                otbReciprocalCoherencyToReciprocalMuellerImageFilterNew
)

ADD_TEST(saTvReciprocalCoherencyToReciprocalMuellerImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvReciprocalCoherencyToMuellerImageFilter.tif
                    ${TEMP}/saTvReciprocalCoherencyToMuellerImageFilter.tif
        otbReciprocalCoherencyToReciprocalMuellerImageFilter
        ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
        ${TEMP}/saTvReciprocalCoherencyToMuellerImageFilter.tif
        )

# Mueller To polarisation degree and power Image Filter
ADD_TEST(saTuMuellerToPolarisationDegreeAndPowerImageFilterNew ${SARPOLARIMETRY_TESTS2}
                otbMuellerToPolarisationDegreeAndPowerImageFilterNew
)

ADD_TEST(saTvMuellerToPolarisationDegreeAndPowerImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
                    ${TEMP}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
        otbMuellerToPolarisationDegreeAndPowerImageFilter
        ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
        ${TEMP}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
        )

# Mueller To Covariance Image Filter
ADD_TEST(saTvMuellerToReciprocalCovarianceFunctor ${SARPOLARIMETRY_TESTS2}
                otbMuellerToReciprocalCovarianceFunctor
)

ADD_TEST(saTuMuellerToReciprocalCovarianceImageFilterNew ${SARPOLARIMETRY_TESTS2}
                otbMuellerToReciprocalCovarianceImageFilterNew
)

ADD_TEST(saTvMuellerToReciprocalCovarianceImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMuellerToMLCImageFilter.tif
                    ${TEMP}/saTvMuellerToMLCImageFilter.tif
        otbMuellerToReciprocalCovarianceImageFilter
        ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
        ${TEMP}/saTvMuellerToMLCImageFilter.tif
        )

# Polarimetric Data
ADD_TEST(saTuPolarimetricDataNew ${SARPOLARIMETRY_TESTS2}
                otbPolarimetricDataNew
)

ADD_TEST(saTvPolarimetricDataTest ${SARPOLARIMETRY_TESTS2}
                otbPolarimetricDataTest
)


# A enrichir
SET(SARPOLARIMETRY_SRCS1
otbSARPolarimetryTests1.cxx
otbPolarimetricSynthesisFunctor.cxx
otbPolarimetricSynthesisFilterNew.cxx
otbPolarimetricSynthesisFilter.cxx
otbHHnHVnVVPolarimetricSynthesisFilter.cxx
otbHHnHVPolarimetricSynthesisFilter.cxx
otbVHnVVPolarimetricSynthesisFilter.cxx
otbMultiChannelsPolarimetricSynthesisFilterNew.cxx
otbMultiChannelsPolarimetricSynthesisFilter.cxx
otbVectorMultiChannelsPolarimetricSynthesisFilter.cxx
otbSinclairToCircularCovarianceMatrixFunctor.cxx
otbSinclairToMuellerMatrixFunctor.cxx
otbSinclairToReciprocalCovarianceMatrixFunctor.cxx
otbSinclairToCoherencyMatrixFunctor.cxx
otbSinclairToReciprocalCircularCovarianceMatrixFunctor.cxx
otbSinclairToCovarianceMatrixFunctor.cxx
otbSinclairToReciprocalCoherencyMatrixFunctor.cxx
otbSinclairImageFilter.cxx
otbSinclairReciprocalImageFilter.cxx
)


SET(SARPOLARIMETRY_SRCS2
otbSARPolarimetryTests2.cxx
otbReciprocalCovarianceToReciprocalCoherencyImageFilterNew.cxx
otbReciprocalCovarianceToReciprocalCoherencyImageFilter.cxx
otbReciprocalCovarianceToCoherencyDegreeImageFilterNew.cxx
otbReciprocalCovarianceToCoherencyDegreeImageFilter.cxx
otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilterNew.cxx
otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter.cxx
otbReciprocalHAlphaImageFilterNew.cxx
otbReciprocalHAlphaImageFilter.cxx
otbReciprocalCoherencyToReciprocalMuellerImageFilterNew.cxx
otbReciprocalCoherencyToReciprocalMuellerImageFilter.cxx
otbMuellerToPolarisationDegreeAndPowerImageFilterNew.cxx
otbMuellerToPolarisationDegreeAndPowerImageFilter.cxx
otbMuellerToReciprocalCovarianceFunctor.cxx
otbMuellerToReciprocalCovarianceImageFilterNew.cxx
otbMuellerToReciprocalCovarianceImageFilter.cxx
otbPolarimetricData.cxx
)


OTB_ADD_EXECUTABLE(otbSARPolarimetryTests1 "${SARPOLARIMETRY_SRCS1}" "OTBSARPolarimetry;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbSARPolarimetryTests2 "${SARPOLARIMETRY_SRCS2}" "OTBSARPolarimetry;OTBIO;OTBTesting")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
