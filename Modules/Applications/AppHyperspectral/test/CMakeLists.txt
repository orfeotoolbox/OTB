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
#----------- HyperspectralUnmixing TESTS ----------------
otb_test_application(NAME  apTvHyHyperspectralUnmixing_UCLS
                     APP  HyperspectralUnmixing
                     OPTIONS -in ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
                			 -ie ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
                 			 -out ${TEMP}/apTvHyHyperspectralUnmixing_UCLS.tif double
                 			 -ua ucls
                     VALID   --compare-image ${EPSILON_9}
                              ${BASELINE}/apTvHyHyperspectralUnmixing_UCLS.tif
                  			  ${TEMP}/apTvHyHyperspectralUnmixing_UCLS.tif)

#----------- VertexComponentAnalysis TESTS ----------------
otb_test_application(NAME  apTvHyVertexComponentAnalysis
                     APP  VertexComponentAnalysis
                     OPTIONS -in ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/hsi_cube.tif
                             -ne 5
                             -rand 121212
                 	     -outendm ${TEMP}/apTvHyVertexComponentAnalysis.tif double
                             VALID   --compare-image ${EPSILON_9}
                                     ${BASELINE}/TvHyVertexComponentAnalysisImage.tif
                  	             ${TEMP}/apTvHyVertexComponentAnalysis.tif
)

#----------- EndmemberNumberEstimation TESTS ----------------
otb_test_application(NAME  apTvHyEndmemberNumberEstimation_vd
                     APP  EndmemberNumberEstimation
                     OPTIONS -in ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/hsi_cube.tif
                             -algo vd
                             -algo.vd.far 0.2
                     TESTENVOPTIONS ${TEMP}/aptTvHyEndmemberNumberEstimation_vd.txt
                     VALID --compare-ascii ${EPSILON_7}
                             ${BASELINE_FILES}/aptTvHyEndmemberNumberEstimation_vd.txt
                       ${TEMP}/aptTvHyEndmemberNumberEstimation_vd.txt
)

otb_test_application(NAME  apTvHyEndmemberNumberEstimation_elm
                     APP  EndmemberNumberEstimation
                     OPTIONS -in ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/hsi_cube.tif
                             -algo elm
                     TESTENVOPTIONS ${TEMP}/aptTvHyEndmemberNumberEstimation_elm.txt
                     VALID --compare-ascii ${EPSILON_7}
                             ${BASELINE_FILES}/aptTvHyEndmemberNumberEstimation_elm.txt
                       ${TEMP}/aptTvHyEndmemberNumberEstimation_elm.txt
)

#----------- LocalRxDetection TESTS ------------------------
otb_test_application(NAME  apTvHyLocalRxDetection
                     APP  LocalRxDetection
                     OPTIONS -in ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/hsi_cube.tif?&bands=1:10
                             -out ${TEMP}/apTvHyLocalRxDetection.tif double
                             -ir 1
                             -er 3
                     VALID   --compare-image ${EPSILON_9}
                              ${BASELINE}/apTvHyLocalRxDetection.tif
                              ${TEMP}/apTvHyLocalRxDetection.tif
)
