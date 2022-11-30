#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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
#----------- SARCalibration TESTS ----------------
otb_test_application(NAME apTvRaSARCalibration_SENTINEL1_old
  APP  SARCalibration
  OPTIONS -in ${INPUTDATA}/SENTINEL1_SLC_S6_1S_extract_1200_1100_300_300.tiff?&geom=${INPUTDATA}/SENTINEL1_SLC_S6_1S_extract_1200_1100_300_300.geom
  -out ${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1.tif
  VALID   --compare-image ${NOTOL}
  ${BASELINE}/raTvSarRadiometricCalibration_SENTINEL1.tif
  ${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1.tif )


otb_test_application(NAME apTvRaSARCalibration_SENTINEL1_recent_noise
  APP  SARCalibration
  OPTIONS -in ${INPUTDATA}/s1b-iw-grd-vh-roi.tif?&geom=${INPUTDATA}/s1b-iw-grd-vh-roi.geom
  -out ${TEMP}/apTvRaSARCalibration_SENTINEL1_recent_noise.tif
  -removenoise 1
  VALID   --compare-image ${NOTOL}
  ${BASELINE}/apTvRaSARCalibration_SENTINEL1_recent_noise.tif
  ${TEMP}/apTvRaSARCalibration_SENTINEL1_recent_noise.tif )


otb_test_application(NAME apTvRaSARCalibration_SENTINEL1_recent
  APP  SARCalibration
  OPTIONS -in ${INPUTDATA}/s1b-iw-grd-vh-roi.tif?&geom=${INPUTDATA}/s1b-iw-grd-vh-roi.geom
  -out ${TEMP}/apTvRaSARCalibration_SENTINEL1_recent.tif
  -removenoise 0
  VALID   --compare-image ${NOTOL}
  ${BASELINE}/apTvRaSARCalibration_SENTINEL1_recent.tif 
  ${TEMP}/apTvRaSARCalibration_SENTINEL1_recent.tif )


if(OTB_DATA_USE_LARGEINPUT)
  otb_test_application(NAME apTvRaSARCalibration_SENTINEL1_PRODUCT_INPUT
    APP  SARCalibration
    OPTIONS -in  LARGEINPUT{SENTINEL1/S1A_S6_SLC__1SSV_20150619T195043/measurement/s1a-s6-slc-vv-20150619t195043-20150619t195101-006447-00887d-001.tiff}
    -out "${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1_PRODUCT_INPUT.tif?&box=1200:1100:256:256"
    VALID   --compare-image ${NOTOL}
    ${BASELINE}/raTvSarRadiometricCalibration_SENTINEL1_PRODUCT_INPUT.tif
    ${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1_PRODUCT_INPUT.tif )
endif()

otb_test_application(NAME apTvRaSARCalibration_RADARSAT2
  APP  SARCalibration
  OPTIONS -in ${INPUTDATA}/radarsat2/RADARSAT2_ALTONA_300_300_VV.tif?&geom=${INPUTDATA}/radarsat2/RADARSAT2_ALTONA_300_300_VV.geom
  -out ${TEMP}/apTvRaSarRadiometricCalibration_RADARSAT2.tif
  VALID   --compare-image ${EPSILON_5}
  ${BASELINE}/raTvSarRadiometricCalibration_RADARSAT2.tif
  ${TEMP}/apTvRaSarRadiometricCalibration_RADARSAT2.tif )

otb_test_application(NAME apTvRaSarDeburst_SENTINEL1
  APP  SARDeburst
  OPTIONS -in ${INPUTDATA}/s1a-iw1-slc-vh-amp_xt.tif
  -out ${TEMP}/apTvRaSarDeburst_SENTINEL1_output.tif uint16
  VALID --compare-image ${NOTOL}
  # Same baseline as filter test
  ${BASELINE}/saTvSarDeburstImageFilterTestOutput.tif
  ${TEMP}/apTvRaSarDeburst_SENTINEL1_output.tif)

otb_test_application(NAME apTvRaSarBurstExtraction_SENTINEL1
  APP  SARBurstExtraction
  OPTIONS -in ${INPUTDATA}/s1a-iw1-slc-vv-20170111_Burst01_amp.tiff
  -out ${TEMP}/apTvRaSarBurstExtraction_SENTINEL1_output.tif uint16
  -burstindex 0
  VALID --compare-image ${NOTOL}
  # Same baseline as filter test
  ${BASELINE}/apTvRaSarBurstExtraction_SENTINEL1_output.tif
  ${TEMP}/apTvRaSarBurstExtraction_SENTINEL1_output.tif)

otb_test_application(NAME apTvRaSarConcatenateBursts_SENTINEL1
  APP  SARConcatenateBursts
  OPTIONS -il ${INPUTDATA}/s1a-iw1-slc-vv-20170111_Burst0_amp.tif ${INPUTDATA}/s1a-iw1-slc-vv-20170111_Burst1_amp.tif
  -insar ${INPUTDATA}/s1a-iw1-slc-vv-20170111_Burst01_amp.tiff
  -out ${TEMP}/apTvRaSarConcatenateBursts_SENTINEL1_output.tif uint16
  VALID --compare-image ${NOTOL}
  # Same baseline as filter test
  ${BASELINE}/apTvRaSarConcatenateBursts_SENTINEL1_output.tif
  ${TEMP}/apTvRaSarConcatenateBursts_SENTINEL1_output.tif)

#----------- SARDecompositions TESTS ----------------

otb_test_application(NAME  apTvSARDecompositionsHAAlpha
APP  SARDecompositions
OPTIONS 
-inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
-inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
-invv ${INPUTDATA}/RSAT_imageryC_VV.tif
-decomp haa
-out ${TEMP}/apTvReciprocalHAlpha.tif

)
    
        
otb_test_application(NAME  apTvSARDecompositionsBarnes
APP  SARDecompositions
OPTIONS 
-inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
-inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
-invv ${INPUTDATA}/RSAT_imageryC_VV.tif
-decomp barnes
-out ${TEMP}/apTvReciprocalBarnes.tif

)

otb_test_application(NAME  apTvSARDecompositionsHuynen
APP  SARDecompositions
OPTIONS 
-inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
-inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
-invv ${INPUTDATA}/RSAT_imageryC_VV.tif
-decomp huynen
-out ${TEMP}/apTvReciprocalHuynen.tif

)

otb_test_application(NAME  apTvSARDecompositionsPauli
APP  SARDecompositions
OPTIONS 
-inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
-inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
-invv ${INPUTDATA}/RSAT_imageryC_VV.tif
-decomp pauli
-out ${TEMP}/apTvReciprocalPauli.tif

)

#----------- SARPolarMatrixConvert TESTS ----------------

#1
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCoherency
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv msinclairtocoherency 
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCoherency.tif
							 
                     )
            
#2                            
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCovariance
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv msinclairtocovariance 
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCovariance.tif
							 
                     )

#3
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCirCovariance
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv msinclairtocircovariance 
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCirCovariance.tif
							 
                     )

#4
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCohToMueller
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mcoherencytomueller
							 -outf ${TEMP}/apTvSARPolarMatrixConvertRecCohToMueller.tif
							 
                     )
                             
                             
#5
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCovToCohDeg
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mcovariancetocoherencydegree
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCovToCohDeg.tif
							 
                     )
                             

#6 
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCovToRecCoh
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mcovariancetocoherency
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCovToRecCoh.tif
							 
                     )

#7 
otb_test_application(NAME  apTvSARPolarMatrixConvertRecLinCovToRecCirCov
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mlinearcovariancetocircularcovariance
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecLinCovToRecCirCov.tif
							 
                     )
                             
                             
#8 
otb_test_application(NAME  apTvSARPolarMatrixConvertMuellerToRecCov
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inf ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
							 -conv muellertomcovariance
							 -outc ${TEMP}/apTvSARPolarMatrixConvertMuellerToRecCov.tif
							 
                     )


#9
otb_test_application(NAME  apTvSARPolarMatrixConvertBiSincToCoherency
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_VH.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv bsinclairtocoherency
							 -outc ${TEMP}/apTvSARPolarMatrixConvertBiSincToCoherency.tif
							 
                     )
                             
                             
#10
otb_test_application(NAME  apTvSARPolarMatrixConvertBiSincToCovariance
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_VH.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv bsinclairtocovariance
							 -outc ${TEMP}/apTvSARPolarMatrixConvertBiSincToCovariance.tif
							 
                     )
                             
#11
otb_test_application(NAME  apTvSARPolarMatrixConvertBiSincToCirCovariance
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_VH.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv bsinclairtocircovariance
							 -outc ${TEMP}/apTvSARPolarMatrixConvertBiSincToCirCovariance.tif
							 
                     )
   
                             
#12 
otb_test_application(NAME  apTvSARPolarMatrixConvertSincToMueller
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_HV.tif #monostatic
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv sinclairtomueller
							 -outf ${TEMP}/apTvSARPolarMatrixConvertSincToMueller.tif
							 
                     )
                             
                             
#13 
otb_test_application(NAME  apTvSARPolarMatrixConvertMuellerToPolDeGPow
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inf ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
							 -conv muellertopoldegandpower
							 -outf ${TEMP}/apTvSARPolarMatrixConvertMuellerToPolDeGPow.tif
							 
                     )

#----------- PolarSynth TESTS ----------------

otb_test_application(NAME  apTvSARPolarSynth
                     APP  SARPolarSynth
                     OPTIONS -in ${INPUTDATA}/RSAT2_AltonaExtract_1000_1000_100_100.tif
               	             -out ${TEMP}/resApMultiPolarimetricSynthesis1.tif
                             -psii 10.0 
                             -khii 0.0
                             -psir 0.0
                             -khir 0.0
                     )

#----------- ComputeModulusAndPhase TESTS ----------------
otb_test_application(NAME apTvUtComputeModulusAndPhase_1inputComplex
APP  ComputeModulusAndPhase
OPTIONS -in ${INPUTDATA}/monobandComplexFloat.tif
-modulus ${TEMP}/apTvUtMod1inputComplex.tif
-phase ${TEMP}/apTvUtPha1inputComplex.tif
VALID   --compare-n-images ${EPSILON_6} 2
${BASELINE}/Mod_monobandComplexFloat.tif
${TEMP}/apTvUtMod1inputComplex.tif
${BASELINE}/Pha_monobandComplexFloat.tif
${TEMP}/apTvUtPha1inputComplex.tif
)

#----------- Despeckle TESTS ----------------

otb_test_application(NAME  apTvDespeckleLee
APP  Despeckle
OPTIONS -in ${INPUTDATA}/GomaAvant.tif
-out ${TEMP}/bfFiltreLee_05_05_12_app.tif
-filter lee
-filter.lee.rad 5
-filter.lee.nblooks 12
VALID   --compare-image ${EPSILON_7}
${BASELINE}/bfFiltreLee_05_05_12.tif
${TEMP}/bfFiltreLee_05_05_12_app.tif)

otb_test_application(NAME  apTvDespeckleFrost
APP  Despeckle
OPTIONS -in ${INPUTDATA}/GomaAvant.tif
-out ${TEMP}/bfFiltreFrost_05_05_12_app.tif
-filter frost
-filter.frost.rad 5
-filter.frost.deramp 0.1
VALID   --compare-image ${EPSILON_7}
${BASELINE}/bfFiltreFrost_05_05_12_app.tif
${TEMP}/bfFiltreFrost_05_05_12_app.tif)

otb_test_application(NAME  apTvDespeckleGammaMAP
APP  Despeckle
OPTIONS -in ${INPUTDATA}/GomaAvant.tif
-out ${TEMP}/bfFiltreGammaMAP_05_05_12_app.tif
-filter gammamap
-filter.gammamap.rad 5
-filter.gammamap.nblooks 12
VALID   --compare-image ${EPSILON_7}
${BASELINE}/bfFiltreGammaMAP_05_05_12.tif
${TEMP}/bfFiltreGammaMAP_05_05_12_app.tif)                      

otb_test_application(NAME  apTvDespeckleKuan
APP  Despeckle
OPTIONS -in ${INPUTDATA}/GomaAvant.tif
-out ${TEMP}/bfFiltreKuan_05_05_12_app.tif
-filter kuan
-filter.kuan.rad 5
-filter.kuan.nblooks 12
VALID   --compare-image ${EPSILON_7}
${BASELINE}/bfFiltreKuan_05_05_12.tif
${TEMP}/bfFiltreKuan_05_05_12_app.tif)
