otb_module_test()

set(OTBPolarimetryTests
otbPolarimetryTestDriver.cxx
otbMuellerToPolarisationDegreeAndPowerImageFilter.cxx
otbVectorMultiChannelsPolarimetricSynthesisFilter.cxx
otbReciprocalHAlphaImageFilter.cxx
otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilterNew.cxx
otbReciprocalCovarianceToReciprocalCoherencyImageFilter.cxx
otbReciprocalHAlphaImageFilterNew.cxx
otbSinclairToCoherencyMatrixFunctor.cxx
otbPolarimetricSynthesisFunctor.cxx
otbMultiChannelsPolarimetricSynthesisFilter.cxx
otbSinclairImageFilter.cxx
otbReciprocalCovarianceToReciprocalCoherencyImageFilterNew.cxx
otbSinclairToCircularCovarianceMatrixFunctor.cxx
otbSinclairReciprocalImageFilter.cxx
otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter.cxx
otbReciprocalCoherencyToReciprocalMuellerImageFilter.cxx
otbSinclairToReciprocalCovarianceMatrixFunctor.cxx
otbReciprocalCovarianceToCoherencyDegreeImageFilterNew.cxx
otbMuellerToReciprocalCovarianceImageFilter.cxx
otbReciprocalCoherencyToReciprocalMuellerImageFilterNew.cxx
otbSinclairToMuellerMatrixFunctor.cxx
otbMuellerToReciprocalCovarianceFunctor.cxx
otbPolarimetricData.cxx
otbMuellerToReciprocalCovarianceImageFilterNew.cxx
otbSinclairToReciprocalCoherencyMatrixFunctor.cxx
otbSinclairToReciprocalCircularCovarianceMatrixFunctor.cxx
otbMultiChannelsPolarimetricSynthesisFilterNew.cxx
otbSinclairToCovarianceMatrixFunctor.cxx
otbReciprocalCovarianceToCoherencyDegreeImageFilter.cxx
otbMuellerToPolarisationDegreeAndPowerImageFilterNew.cxx
)

add_executable(otbPolarimetryTestDriver ${OTBPolarimetryTests})
target_link_libraries(otbPolarimetryTestDriver ${OTBPolarimetry-Test_LIBRARIES})
otb_module_target_label(otbPolarimetryTestDriver)

# Tests Declaration

otb_add_test(NAME saTvMuellerToPolarisationDegreeAndPowerImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
  ${TEMP}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
  otbMuellerToPolarisationDegreeAndPowerImageFilter
  ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
  ${TEMP}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
  )

otb_add_test(NAME saTvVectorMultiChannelsPolarimetricSynthesisFilter COMMAND otbPolarimetryTestDriver
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

otb_add_test(NAME saTvReciprocalHAlphaImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvReciprocalHAlphaImageFilter.tif
  ${TEMP}/saTvReciprocalHAlphaImageFilter.tif
  otbReciprocalHAlphaImageFilter
  ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  ${TEMP}/saTvReciprocalHAlphaImageFilter.tif
  )

otb_add_test(NAME saTuReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilterNew COMMAND otbPolarimetryTestDriver
  otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilterNew
  )

otb_add_test(NAME saTvReciprocalCovarianceToReciprocalCoherencyImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  ${TEMP}/saTvMLCToCoherencyImageFilter.tif
  otbReciprocalCovarianceToReciprocalCoherencyImageFilter
  ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  ${TEMP}/saTvMLCToCoherencyImageFilter.tif
  )

otb_add_test(NAME saTuReciprocalHAlphaImageFilterNew COMMAND otbPolarimetryTestDriver
  otbReciprocalHAlphaImageFilterNew
  )

otb_add_test(NAME saTuSinclairToCoherencyMatrixFunctor COMMAND otbPolarimetryTestDriver
  otbSinclairToCoherencyMatrixFunctor
  )

otb_add_test(NAME saTuPolarimetricSynthesisFunctor COMMAND otbPolarimetryTestDriver
  otbPolarimetricSynthesisFunctor )

otb_add_test(NAME saTvMultiChannelsPolarimetricSynthesisFilter COMMAND otbPolarimetryTestDriver
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

otb_add_test(NAME saTvMultiChannelsPolarimetricSynthesisFilter_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMultiPolarimetricSynthesis2.tif
  ${TEMP}/resMultiPolarimetricSynthesis2.tif
  otbMultiChannelsPolarimetricSynthesisFilter
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/resMultiPolarimetricSynthesis2.tif
  32.0     # PsiI
  22.0     # KhiI
  58.0     # PsiR
  45.0     # KhiR
  )

otb_add_test(NAME saTvSinclairImageFilter_SinclairToMueller COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToMueller.tif
  otbSinclairImageFilter
  SinclairToMuellerMatrix
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToMueller.tif
  )
  
otb_add_test(NAME saTvSinclairImageFilter_SinclairToMueller_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToMueller.tif
  otbSinclairImageFilter
  SinclairToMuellerMatrix
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToMueller.tif
  )

otb_add_test(NAME saTvSinclairImageFilter_SinclairToCovariance COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCovariance.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCovariance.tif
  otbSinclairImageFilter
  SinclairToCovarianceMatrix
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCovariance.tif
  )
  
otb_add_test(NAME saTvSinclairImageFilter_SinclairToCovariance_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCovariance.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCovariance.tif
  otbSinclairImageFilter
  SinclairToCovarianceMatrix
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCovariance.tif
  )

otb_add_test(NAME saTuSinclairImageFilterNew COMMAND otbPolarimetryTestDriver
  otbSinclairImageFilterNew
  )

otb_add_test(NAME saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix.tif
  otbSinclairImageFilter
  SinclairToCircularCovarianceMatrix
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix.tif
  )
  
otb_add_test(NAME saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix.tif
  otbSinclairImageFilter
  SinclairToCircularCovarianceMatrix
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix.tif
  )

otb_add_test(NAME saTvSinclairImageFilter_SinclairToCoherency COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCoherency.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCoherency.tif
  otbSinclairImageFilter
  SinclairToCoherencyMatrix
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCoherency.tif
  )
  
otb_add_test(NAME saTvSinclairImageFilter_SinclairToCoherency_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCoherency.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCoherency.tif
  otbSinclairImageFilter
  SinclairToCoherencyMatrix
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCoherency.tif
  )

otb_add_test(NAME saTuReciprocalCovarianceToReciprocalCoherencyImageFilterNew COMMAND otbPolarimetryTestDriver
  otbReciprocalCovarianceToReciprocalCoherencyImageFilterNew
  )

otb_add_test(NAME saTuSinclairToCircularCovarianceMatrixFunctor COMMAND otbPolarimetryTestDriver
  otbSinclairToCircularCovarianceMatrixFunctor
  )

otb_add_test(NAME saTuSinclairReciprocalImageFilterNew COMMAND otbPolarimetryTestDriver
  otbSinclairReciprocalImageFilterNew
  )

otb_add_test(NAME saTvSinclairReciprocalImageFilter_SinclairToReciprocalCovariance COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  otbSinclairReciprocalImageFilter
  SinclairToReciprocalCovarianceMatrix
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  )
  
otb_add_test(NAME saTvSinclairReciprocalImageFilter_SinclairToReciprocalCovariance_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  otbSinclairReciprocalImageFilter
  SinclairToReciprocalCovarianceMatrix
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  )

otb_add_test(NAME saTvSinclairReciprocalImageFilter_SinclairToReciprocalCircularCovarianceMatrix COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
  otbSinclairReciprocalImageFilter
  SinclairToReciprocalCircularCovarianceMatrix
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
  )
  
  
otb_add_test(NAME saTvSinclairReciprocalImageFilter_SinclairToReciprocalCircularCovarianceMatrix_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
  otbSinclairReciprocalImageFilter
  SinclairToReciprocalCircularCovarianceMatrix
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
  )

otb_add_test(NAME saTvSinclairReciprocalImageFilter_SinclairToReciprocalCoherency COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  otbSinclairReciprocalImageFilter
  SinclairToReciprocalCoherencyMatrix
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  )
  
otb_add_test(NAME saTvSinclairReciprocalImageFilter_SinclairToReciprocalCoherency_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  otbSinclairReciprocalImageFilter
  SinclairToReciprocalCoherencyMatrix
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  )
 

otb_add_test(NAME saTvReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
  ${TEMP}/saTvMLCToCircularCoherencyDegreeImageFilter.tif
  otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter
  ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  ${TEMP}/saTvMLCToCircularCoherencyDegreeImageFilter.tif
  )

otb_add_test(NAME saTvReciprocalCoherencyToReciprocalMuellerImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif 
  ${TEMP}/saTvReciprocalCoherencyToMuellerImageFilter.tif
  otbReciprocalCoherencyToReciprocalMuellerImageFilter
  ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  ${TEMP}/saTvReciprocalCoherencyToMuellerImageFilter.tif
  )

otb_add_test(NAME saTuSinclairToReciprocalCovarianceMatrixFunctor COMMAND otbPolarimetryTestDriver
  otbSinclairToReciprocalCovarianceMatrixFunctor
  )

otb_add_test(NAME saTuReciprocalCovarianceToCoherencyDegreeImageFilterNew COMMAND otbPolarimetryTestDriver
  otbReciprocalCovarianceToCoherencyDegreeImageFilterNew
  )

otb_add_test(NAME saTvMuellerToReciprocalCovarianceImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  ${TEMP}/saTvMuellerToMLCImageFilter.tif
  otbMuellerToReciprocalCovarianceImageFilter
  ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
  ${TEMP}/saTvMuellerToMLCImageFilter.tif
  )

otb_add_test(NAME saTuReciprocalCoherencyToReciprocalMuellerImageFilterNew COMMAND otbPolarimetryTestDriver
  otbReciprocalCoherencyToReciprocalMuellerImageFilterNew
  )

otb_add_test(NAME saTuSinclairToMuellerMatrixFunctor COMMAND otbPolarimetryTestDriver
  otbSinclairToMuellerMatrixFunctor
  )

otb_add_test(NAME saTvMuellerToReciprocalCovarianceFunctor COMMAND otbPolarimetryTestDriver
  otbMuellerToReciprocalCovarianceFunctor
  )

otb_add_test(NAME saTuPolarimetricDataNew COMMAND otbPolarimetryTestDriver
  otbPolarimetricDataNew
  )

otb_add_test(NAME saTvPolarimetricDataTest COMMAND otbPolarimetryTestDriver
  otbPolarimetricDataTest
  )

otb_add_test(NAME saTuMuellerToReciprocalCovarianceImageFilterNew COMMAND otbPolarimetryTestDriver
  otbMuellerToReciprocalCovarianceImageFilterNew
  )

otb_add_test(NAME saTuSinclairToReciprocalCoherencyMatrixFunctor COMMAND otbPolarimetryTestDriver
  otbSinclairToReciprocalCoherencyMatrixFunctor
  )

otb_add_test(NAME saTuSinclairToReciprocalCircularCovarianceMatrixFunctor COMMAND otbPolarimetryTestDriver
  otbSinclairToReciprocalCircularCovarianceMatrixFunctor
  )

otb_add_test(NAME saTuMultiChannelsPolarimetricSynthesisFilterNew COMMAND otbPolarimetryTestDriver
  otbMultiChannelsPolarimetricSynthesisFilterNew
  )

otb_add_test(NAME saTuSinclairToCovarianceMatrixFunctor COMMAND otbPolarimetryTestDriver
  otbSinclairToCovarianceMatrixFunctor
  )

otb_add_test(NAME saTvReciprocalCovarianceToCoherencyDegreeImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/saTvMLCToCoherencyDegreeImageFilter.tif
  ${TEMP}/saTvMLCToCoherencyDegreeImageFilter.tif
  otbReciprocalCovarianceToCoherencyDegreeImageFilter
  ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  ${TEMP}/saTvMLCToCoherencyDegreeImageFilter.tif
  )

otb_add_test(NAME saTuMuellerToPolarisationDegreeAndPowerImageFilterNew COMMAND otbPolarimetryTestDriver
  otbMuellerToPolarisationDegreeAndPowerImageFilterNew
  )

