#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
#----------- BundleToPerfectSensor TESTS ----------------
otb_test_application(NAME apTvPrBundleToPerfectSensor
                     APP BundleToPerfectSensor
                     OPTIONS -inp  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -inxs ${INPUTDATA}/QB_Toulouse_Ortho_XS_ROI_170x230.tif
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory
                             -out ${TEMP}/apTvPrBundleToPerfectSensor.tif int16
                     VALID  --compare-image ${EPSILON_7}
                        ${BASELINE}/apTvPrBundleToPerfectSensor.tif
                        ${TEMP}/apTvPrBundleToPerfectSensor.tif)
                      
otb_test_application(NAME apTvPrBundleToPerfectSensor_phr
                     APP BundleToPerfectSensor
                     OPTIONS -inp  ${INPUTDATA}/phr_pan.tif
                             -inxs ${INPUTDATA}/phr_xs.tif
                             -out ${TEMP}/apTvPrBundleToPerfectSensorPHR.tif int16
                     VALID  --compare-image ${EPSILON_7}
                        ${BASELINE}/apTvPrBundleToPerfectSensorPHR.tif
                        ${TEMP}/apTvPrBundleToPerfectSensorPHR.tif)

#----------- Pansharpening TESTS ----------------
otb_test_application(NAME  apTvFuPansharpening_LMVM
                     APP  Pansharpening
                     OPTIONS -inp ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -inxs ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                       -out ${TEMP}/apTvFuPanSharpeningLmvm.tif double
                       -method lmvm
                             -method.lmvm.radiusx 5
                             -method.lmvm.radiusy 5
                     VALID   --compare-image ${EPSILON_6}
                             ${BASELINE}/fuTvLmvmPanSharpeningFusion.tif
                            ${TEMP}/apTvFuPanSharpeningLmvm.tif
                     )

otb_test_application(NAME  apTvFuPansharpening_Bayes
                     APP  Pansharpening
                     OPTIONS -inp ${INPUTDATA}/panchro.tif
                             -inxs ${INPUTDATA}/multiSpectInterp.tif
                       -out ${TEMP}/apTvFuPanSharpeningBayes.tif double
                       -method bayes
                     VALID   --compare-image ${EPSILON_6}
                             ${BASELINE}/apTvFuPanSharpeningBayes.tif
                            ${TEMP}/apTvFuPanSharpeningBayes.tif
                     )

otb_test_application(NAME  apTvFuPansharpening_RCS
                     APP  Pansharpening
                     OPTIONS -inp ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -inxs ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                       -out ${TEMP}/apTvFuPanSharpeningRcs.tif double
                       -method rcs
                     VALID   --compare-image ${EPSILON_6}
                             ${BASELINE}/fuTvRcsPanSharpeningFusion.tif
                            ${TEMP}/apTvFuPanSharpeningRcs.tif
                     )

