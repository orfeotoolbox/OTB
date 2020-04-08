#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

otb_module_test()

set(OTBPolarimetryTests
otbPolarimetryTestDriver.cxx
otbMuellerToPolarisationDegreeAndPowerImageFilter.cxx
otbVectorMultiChannelsPolarimetricSynthesisFilter.cxx
otbReciprocalHAlphaImageFilter.cxx
otbReciprocalCovarianceToReciprocalCoherencyImageFilter.cxx
otbSinclairToCoherencyMatrixFunctor.cxx
otbPolarimetricSynthesisFunctor.cxx
otbMultiChannelsPolarimetricSynthesisFilter.cxx
otbSinclairImageFilter.cxx
otbSinclairToCircularCovarianceMatrixFunctor.cxx
otbSinclairReciprocalImageFilter.cxx
otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter.cxx
otbReciprocalCoherencyToReciprocalMuellerImageFilter.cxx
otbSinclairToReciprocalCovarianceMatrixFunctor.cxx
otbMuellerToReciprocalCovarianceImageFilter.cxx
otbSinclairToMuellerMatrixFunctor.cxx
otbMuellerToReciprocalCovarianceFunctor.cxx
otbPolarimetricData.cxx
otbSinclairToReciprocalCoherencyMatrixFunctor.cxx
otbSinclairToReciprocalCircularCovarianceMatrixFunctor.cxx
otbSinclairToCovarianceMatrixFunctor.cxx
otbReciprocalCovarianceToCoherencyDegreeImageFilter.cxx
otbReciprocalBarnesDecomp.cxx
otbReciprocalHuynenDecomp.cxx
otbReciprocalPauliDecomp.cxx
)

add_executable(otbPolarimetryTestDriver ${OTBPolarimetryTests})
target_link_libraries(otbPolarimetryTestDriver ${OTBPolarimetry-Test_LIBRARIES})
otb_module_target_label(otbPolarimetryTestDriver)

# Tests Declaration

otb_add_test(NAME saTvMuellerToPolarisationDegreeAndPowerImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
  ${TEMP}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
  otbMuellerToPolarisationDegreeAndPowerImageFilter
  ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
  ${TEMP}/saTvMuellerToPolarisationDegreeAndPowerImageFilter.tif
  )

otb_add_test(NAME saTvVectorMultiChannelsPolarimetricSynthesisFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvMultiPolarimetricSynthesis1.tif
  ${TEMP}/resMultiPolarimetricSynthesis1.tif
  otbVectorMultiChannelsPolarimetricSynthesisFilter
  ${INPUTDATA}/RSAT2_AltonaExtract_1000_1000_100_100.tif
  ${TEMP}/resMultiPolarimetricSynthesis1.tif
  10.0     # PsiI
  0.0     # KhiI
  0.0     # PsiR
  0.0     # KhiR
  )

otb_add_test(NAME saTvReciprocalHAlphaImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvReciprocalHAlphaImageFilter.tif
  ${TEMP}/saTvReciprocalHAlphaImageFilter.tif
  otbReciprocalHAlphaImageFilter
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  5
  ${TEMP}/saTvReciprocalHAlphaImageFilter.tif
  )
  
otb_add_test(NAME saTvReciprocalBarnesDecompImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvReciprocalBarnesDecompImageFilter.tif
  ${TEMP}/saTvReciprocalBarnesDecompImageFilter.tif
  otbReciprocalBarnesDecompImageFilter
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  5
  ${TEMP}/saTvReciprocalBarnesDecompImageFilter.tif
  )
  
otb_add_test(NAME saTvReciprocalHuynenDecompImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvReciprocalHuynenDecompImageFilter.tif
  ${TEMP}/saTvReciprocalHuynenDecompImageFilter.tif
  otbReciprocalHuynenDecompImageFilter
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  5
  ${TEMP}/saTvReciprocalHuynenDecompImageFilter.tif
  )
  
otb_add_test(NAME saTvReciprocalPauliDecompImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvReciprocalPauliDecompImageFilter.tif
  ${TEMP}/saTvReciprocalPauliDecompImageFilter.tif
  otbReciprocalPauliDecompImageFilter
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvReciprocalPauliDecompImageFilter.tif
  )


otb_add_test(NAME saTvReciprocalCovarianceToReciprocalCoherencyImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  ${TEMP}/saTvMLCToCoherencyImageFilter.tif
  otbReciprocalCovarianceToReciprocalCoherencyImageFilter
  ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  ${TEMP}/saTvMLCToCoherencyImageFilter.tif
  )

  
  
  

otb_add_test(NAME saTuSinclairToCoherencyMatrixFunctor COMMAND otbPolarimetryTestDriver
  otbSinclairToCoherencyMatrixFunctor
  )

otb_add_test(NAME saTuPolarimetricSynthesisFunctor COMMAND otbPolarimetryTestDriver
  otbPolarimetricSynthesisFunctor )

otb_add_test(NAME saTvMultiChannelsPolarimetricSynthesisFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvMultiPolarimetricSynthesis2.tif
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
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvMultiPolarimetricSynthesis2.tif
  ${TEMP}/resMultiPolarimetricSynthesis2_C.tif
  otbMultiChannelsPolarimetricSynthesisFilter
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/resMultiPolarimetricSynthesis2_C.tif
  32.0     # PsiI
  22.0     # KhiI
  58.0     # PsiR
  45.0     # KhiR
  )

otb_add_test(NAME saTvSinclairImageFilter_SinclairToMueller COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToMueller.tif
  otbSinclairImageFilter
  SinclairToMuellerMatrix
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToMueller.tif
  )
  
otb_add_test(NAME saTvSinclairImageFilter_SinclairToMueller_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToMueller_C.tif
  otbSinclairImageFilter
  SinclairToMuellerMatrix
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToMueller_C.tif
  )

otb_add_test(NAME saTvSinclairImageFilter_SinclairToCovariance COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCovariance.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCovariance.tif
  otbSinclairImageFilter
  SinclairToCovarianceMatrix
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCovariance.tif
  )
  
otb_add_test(NAME saTvSinclairImageFilter_SinclairToCovariance_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCovariance.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCovariance_C.tif
  otbSinclairImageFilter
  SinclairToCovarianceMatrix
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCovariance_C.tif
  )


otb_add_test(NAME saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix.tif
  otbSinclairImageFilter
  SinclairToCircularCovarianceMatrix
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix.tif
  )
  
otb_add_test(NAME saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix_C.tif
  otbSinclairImageFilter
  SinclairToCircularCovarianceMatrix
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCircularCovarianceMatrix_C.tif
  )

otb_add_test(NAME saTvSinclairImageFilter_SinclairToCoherency COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCoherency.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCoherency.tif
  otbSinclairImageFilter
  SinclairToCoherencyMatrix
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCoherency.tif
  )
  
otb_add_test(NAME saTvSinclairImageFilter_SinclairToCoherency_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToCoherency.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCoherency_C.tif
  otbSinclairImageFilter
  SinclairToCoherencyMatrix
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToCoherency_C.tif
  )


otb_add_test(NAME saTuSinclairToCircularCovarianceMatrixFunctor COMMAND otbPolarimetryTestDriver
  otbSinclairToCircularCovarianceMatrixFunctor
  )


otb_add_test(NAME saTvSinclairReciprocalImageFilter_SinclairToReciprocalCovariance COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  otbSinclairReciprocalImageFilter
  SinclairToReciprocalCovarianceMatrix
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  )
  
otb_add_test(NAME saTvSinclairReciprocalImageFilter_SinclairToReciprocalCovariance_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCovariance_C.tif
  otbSinclairReciprocalImageFilter
  SinclairToReciprocalCovarianceMatrix
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCovariance_C.tif
  )

otb_add_test(NAME saTvSinclairReciprocalImageFilter_SinclairToReciprocalCircularCovarianceMatrix COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
  otbSinclairReciprocalImageFilter
  SinclairToReciprocalCircularCovarianceMatrix
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
  )
  
  
otb_add_test(NAME saTvSinclairReciprocalImageFilter_SinclairToReciprocalCircularCovarianceMatrix_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix_C.tif
  otbSinclairReciprocalImageFilter
  SinclairToReciprocalCircularCovarianceMatrix
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix_C.tif
  )

otb_add_test(NAME saTvSinclairReciprocalImageFilter_SinclairToReciprocalCoherency COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  otbSinclairReciprocalImageFilter
  SinclairToReciprocalCoherencyMatrix
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  )
  
otb_add_test(NAME saTvSinclairReciprocalImageFilter_SinclairToReciprocalCoherency_C COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCoherency_C.tif
  otbSinclairReciprocalImageFilter
  SinclairToReciprocalCoherencyMatrix
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${INPUTDATA}/RSAT_imageryC_HV.tif
  ${INPUTDATA}/RSAT_imageryC_VV.tif
  ${TEMP}/saTvSinclairImageFilter_SinclairToReciprocalCoherency_C.tif
  )
 

otb_add_test(NAME saTvReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCircularCovarianceMatrix.tif
  ${TEMP}/saTvMLCToCircularCoherencyDegreeImageFilter.tif
  otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter
  ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  ${TEMP}/saTvMLCToCircularCoherencyDegreeImageFilter.tif
  )

otb_add_test(NAME saTvReciprocalCoherencyToReciprocalMuellerImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif 
  ${TEMP}/saTvReciprocalCoherencyToMuellerImageFilter.tif
  otbReciprocalCoherencyToReciprocalMuellerImageFilter
  ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCoherency.tif
  ${TEMP}/saTvReciprocalCoherencyToMuellerImageFilter.tif
  )

otb_add_test(NAME saTuSinclairToReciprocalCovarianceMatrixFunctor COMMAND otbPolarimetryTestDriver
  otbSinclairToReciprocalCovarianceMatrixFunctor
  )


otb_add_test(NAME saTvMuellerToReciprocalCovarianceImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  ${TEMP}/saTvMuellerToMLCImageFilter.tif
  otbMuellerToReciprocalCovarianceImageFilter
  ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
  ${TEMP}/saTvMuellerToMLCImageFilter.tif
  )


otb_add_test(NAME saTuSinclairToMuellerMatrixFunctor COMMAND otbPolarimetryTestDriver
  otbSinclairToMuellerMatrixFunctor
  )

otb_add_test(NAME saTvMuellerToReciprocalCovarianceFunctor COMMAND otbPolarimetryTestDriver
  otbMuellerToReciprocalCovarianceFunctor
  )


otb_add_test(NAME saTvPolarimetricDataTest COMMAND otbPolarimetryTestDriver
  otbPolarimetricDataTest
  )


otb_add_test(NAME saTuSinclairToReciprocalCoherencyMatrixFunctor COMMAND otbPolarimetryTestDriver
  otbSinclairToReciprocalCoherencyMatrixFunctor
  )

otb_add_test(NAME saTuSinclairToReciprocalCircularCovarianceMatrixFunctor COMMAND otbPolarimetryTestDriver
  otbSinclairToReciprocalCircularCovarianceMatrixFunctor
  )


otb_add_test(NAME saTuSinclairToCovarianceMatrixFunctor COMMAND otbPolarimetryTestDriver
  otbSinclairToCovarianceMatrixFunctor
  )

otb_add_test(NAME saTvReciprocalCovarianceToCoherencyDegreeImageFilter COMMAND otbPolarimetryTestDriver
  --compare-image ${EPSILON_7}   ${BASELINE}/saTvMLCToCoherencyDegreeImageFilter.tif
  ${TEMP}/saTvMLCToCoherencyDegreeImageFilter.tif
  otbReciprocalCovarianceToCoherencyDegreeImageFilter
  ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
  ${TEMP}/saTvMLCToCoherencyDegreeImageFilter.tif
  )


