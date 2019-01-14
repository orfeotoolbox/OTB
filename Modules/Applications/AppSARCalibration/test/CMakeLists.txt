#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
otb_test_application(NAME apTvRaSARCalibration_SENTINEL1
  APP  SARCalibration
  OPTIONS -in ${INPUTDATA}/SENTINEL1_SLC_S6_1S_extract_1200_1100_300_300.tiff?&geom=${INPUTDATA}/SENTINEL1_SLC_S6_1S_extract_1200_1100_300_300.geom
  -out ${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1.tif
  VALID   --compare-image ${NOTOL}
  ${BASELINE}/raTvSarRadiometricCalibration_SENTINEL1.tif
  ${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1.tif )

if(OTB_DATA_USE_LARGEINPUT)
  otb_test_application(NAME apTvRaSARCalibration_SENTINEL1_PRODUCT_INPUT
    APP  SARCalibration
    OPTIONS -in  LARGEINPUT{SENTINEL1/S1A_S6_SLC__1SSV_20150619T195043/measurement/s1a-s6-slc-vv-20150619t195043-20150619t195101-006447-00887d-001.tiff}
    -out "${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1_PRODUCT_INPUT.tif?box=1200:1100:256:256"
    VALID   --compare-image ${NOTOL}
    ${BASELINE}/raTvSarRadiometricCalibration_SENTINEL1_PRODUCT_INPUT.tif
    ${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1_PRODUCT_INPUT.tif )
endif()

otb_test_application(NAME apTvRaSARCalibration_RADARSAT2
  APP  SARCalibration
  OPTIONS -in ${INPUTDATA}/RADARSAT2_ALTONA_300_300_VV.tif?&geom=${INPUTDATA}/RADARSAT2_ALTONA_300_300_VV.geom
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
