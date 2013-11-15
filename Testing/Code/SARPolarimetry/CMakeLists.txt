
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(SARPOLARIMETRY_TESTS1 ${CXX_TEST_PATH}/otbSARPolarimetryTests1)
set(SARPOLARIMETRY_TESTS2 ${CXX_TEST_PATH}/otbSARPolarimetryTests2)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbSARPolarimetry_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

add_test(saTuPolarimetricSynthesisFunctor ${SARPOLARIMETRY_TESTS1}
        otbPolarimetricSynthesisFunctor )


add_test(saTuMultiChannelsPolarimetricSynthesisFilterNew ${SARPOLARIMETRY_TESTS1}
        otbMultiChannelsPolarimetricSynthesisFilterNew
        )

# With vector image HH, HV, VH and VV channels
if(OTB_DATA_USE_LARGEINPUT)
add_test(saTvVectorMultiChannelsPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
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
endif()


# With 3 images HH, HV and VV channels turned into vectorImage
add_test(saTvMultiChannelsPolarimetricSynthesisFilter ${SARPOLARIMETRY_TESTS1}
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
add_test(saTuSinclairToCircularCovarianceMatrixFunctor ${SARPOLARIMETRY_TESTS1}
                otbSinclairToCircularCovarianceMatrixFunctor
)

# otbSinclairToMuellerMatrixFunctor
add_test(saTuSinclairToMuellerMatrixFunctor ${SARPOLARIMETRY_TESTS1}
                otbSinclairToMuellerMatrixFunctor
)

# otbSinclairToReciprocalCovariancMatrixeFunctor
add_test(saTuSinclairToReciprocalCovarianceMatrixFunctor ${SARPOLARIMETRY_TESTS1}
                otbSinclairToReciprocalCovarianceMatrixFunctor
)

# otbSinclairToCoherencyFunctor
add_test(saTuSinclairToCoherencyMatrixFunctor ${SARPOLARIMETRY_TESTS1}
                otbSinclairToCoherencyMatrixFunctor
)

# otbSinclairToReciprocalCircularCovarianceMatrixFunctor
add_test(saTuSinclairToReciprocalCircularCovarianceMatrixFunctor ${SARPOLARIMETRY_TESTS1}
                otbSinclairToReciprocalCircularCovarianceMatrixFunctor
)

# otbSinclairToCovarianceFunctor
add_test(saTuSinclairToCovarianceMatrixFunctor ${SARPOLARIMETRY_TESTS1}
                otbSinclairToCovarianceMatrixFunctor
)

# otbSinclairToReciprocalCoherencyFunctor
add_test(saTuSinclairToReciprocalCoherencyMatrixFunctor ${SARPOLARIMETRY_TESTS1}
                otbSinclairToReciprocalCoherencyMatrixFunctor
)

add_test(saTuSinclairImageFilterNew ${SARPOLARIMETRY_TESTS1}
                otbSinclairImageFilterNew
)

# Sinclair To Covariance Image Filter
add_test(saTvSinclairImageFilter_SinclairToCovariance ${SARPOLARIMETRY_TESTS1}
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
add_test(saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix ${SARPOLARIMETRY_TESTS1}
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
add_test(saTvSinclairImageFilter_SinclairToCoherency ${SARPOLARIMETRY_TESTS1}
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
add_test(saTvSinclairImageFilter_SinclairToMueller ${SARPOLARIMETRY_TESTS1}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
                    ${TEMP}/saTvSinclairImageFilter_SinclairToMueller.tif
        otbSinclairImageFilter
        SinclairToMuellerMatrix
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VV.tif
        ${TEMP}/saTvSinclairImageFilter_SinclairToMueller.tif
        )


add_test(saTuSinclairReciprocalImageFilterNew ${SARPOLARIMETRY_TESTS1}
                otbSinclairReciprocalImageFilterNew
)

# Sinclair To Reciprocal Covariance Image Filter
add_test(saTvSinclairReciprocalImageFilter_SinclairToReciprocalCovariance ${SARPOLARIMETRY_TESTS1}
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
add_test(saTvSinclairReciprocalImageFilter_SinclairToReciprocalCircularCovarianceMatrix ${SARPOLARIMETRY_TESTS1}
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
add_test(saTvSinclairReciprocalImageFilter_SinclairToReciprocalCoherency ${SARPOLARIMETRY_TESTS1}
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
add_test(saTuReciprocalCovarianceToReciprocalCoherencyImageFilterNew ${SARPOLARIMETRY_TESTS2}
                otbReciprocalCovarianceToReciprocalCoherencyImageFilterNew
)

add_test(saTvReciprocalCovarianceToReciprocalCoherencyImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMLCToCoherencyImageFilter.tif
                    ${TEMP}/saTvMLCToCoherencyImageFilter.tif
        otbReciprocalCovarianceToReciprocalCoherencyImageFilter
        ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
        ${TEMP}/saTvMLCToCoherencyImageFilter.tif
        )

# Reciprocal Covariance To Coherency Degree Image Filter
add_test(saTuReciprocalCovarianceToCoherencyDegreeImageFilterNew ${SARPOLARIMETRY_TESTS2}
                otbReciprocalCovarianceToCoherencyDegreeImageFilterNew
)

add_test(saTvReciprocalCovarianceToCoherencyDegreeImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMLCToCoherencyDegreeImageFilter.tif
                    ${TEMP}/saTvMLCToCoherencyDegreeImageFilter.tif
        otbReciprocalCovarianceToCoherencyDegreeImageFilter
        ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
        ${TEMP}/saTvMLCToCoherencyDegreeImageFilter.tif
        )

# Reciprocal Covariance To Circular Coherency Degree Image Filter
add_test(saTuReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilterNew ${SARPOLARIMETRY_TESTS2}
                otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilterNew
)

add_test(saTvReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMLCToCircularCoherencyDegreeImageFilter.tif
                    ${TEMP}/saTvMLCToCircularCoherencyDegreeImageFilter.tif
        otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter
        ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
        ${TEMP}/saTvMLCToCircularCoherencyDegreeImageFilter.tif
        )

# Reciprocal H Alpha Image Filter
add_test(saTuReciprocalHAlphaImageFilterNew ${SARPOLARIMETRY_TESTS2}
                otbReciprocalHAlphaImageFilterNew
)

add_test(saTvReciprocalHAlphaImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvReciprocalHAlphaImageFilter.tif
                    ${TEMP}/saTvReciprocalHAlphaImageFilter.tif
        otbReciprocalHAlphaImageFilter
        ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
        ${TEMP}/saTvReciprocalHAlphaImageFilter.tif
        )

# Reciprocal Coherency To Mueller Image Filter
add_test(saTuReciprocalCoherencyToReciprocalMuellerImageFilterNew ${SARPOLARIMETRY_TESTS2}
                otbReciprocalCoherencyToReciprocalMuellerImageFilterNew
)

add_test(saTvReciprocalCoherencyToReciprocalMuellerImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvReciprocalCoherencyToMuellerImageFilter.tif
                    ${TEMP}/saTvReciprocalCoherencyToMuellerImageFilter.tif
        otbReciprocalCoherencyToReciprocalMuellerImageFilter
        ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
        ${TEMP}/saTvReciprocalCoherencyToMuellerImageFilter.tif
        )

# Mueller To polarisation degree and power Image Filter
add_test(saTuMuellerToPolarisationDegreeAndPowerImageFilterNew ${SARPOLARIMETRY_TESTS2}
                otbMuellerToPolarisationDegreeAndPowerImageFilterNew
)

add_test(saTvMuellerToPolarisationDegreeAndPowerImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
                    ${TEMP}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
        otbMuellerToPolarisationDegreeAndPowerImageFilter
        ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
        ${TEMP}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
        )

# Mueller To Covariance Image Filter
add_test(saTvMuellerToReciprocalCovarianceFunctor ${SARPOLARIMETRY_TESTS2}
                otbMuellerToReciprocalCovarianceFunctor
)

add_test(saTuMuellerToReciprocalCovarianceImageFilterNew ${SARPOLARIMETRY_TESTS2}
                otbMuellerToReciprocalCovarianceImageFilterNew
)

add_test(saTvMuellerToReciprocalCovarianceImageFilter ${SARPOLARIMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMuellerToMLCImageFilter.tif
                    ${TEMP}/saTvMuellerToMLCImageFilter.tif
        otbMuellerToReciprocalCovarianceImageFilter
        ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
        ${TEMP}/saTvMuellerToMLCImageFilter.tif
        )

# Polarimetric Data
add_test(saTuPolarimetricDataNew ${SARPOLARIMETRY_TESTS2}
                otbPolarimetricDataNew
)

add_test(saTvPolarimetricDataTest ${SARPOLARIMETRY_TESTS2}
                otbPolarimetricDataTest
)


# A enrichir
set(SARPOLARIMETRY_SRCS1
otbSARPolarimetryTests1.cxx
otbPolarimetricSynthesisFunctor.cxx
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


set(SARPOLARIMETRY_SRCS2
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

endif()
