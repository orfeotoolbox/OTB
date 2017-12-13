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
#----------- DimensionalityReduction TESTS ----------------
otb_test_application(NAME   apTvFEDimensionalityReductionMAF
                     APP  DimensionalityReduction
                     OPTIONS -in ${INPUTDATA}/cupriteSubHsi.tif
                             -out ${TEMP}/apTvFEDimensionalityReductionMAF.tif
                             -method maf
                     VALID   --compare-image 0.025
                             ${BASELINE}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
                             ${TEMP}/apTvFEDimensionalityReductionMAF.tif)

otb_test_application(NAME   apTvFEDimensionalityReductionPCA
                     APP  DimensionalityReduction
                     OPTIONS -in ${INPUTDATA}/cupriteSubHsi.tif
                             -out ${TEMP}/apTvChDimensionalityReductionPCA.tif
                             -method pca
                     VALID   --compare-image 0.025
                             ${BASELINE}/bfTvPCAImageFilter3.tif
                             ${TEMP}/apTvChDimensionalityReductionPCA.tif)

#----------- TrainDimensionalityReduction TESTS ----------------
otb_test_application(NAME apTvDrTrainDimensionalityReductionAE
  APP TrainDimensionalityReduction
  OPTIONS -io.vd ${INPUTDATA}/cuprite_samples.sqlite
          -io.out ${TEMP}/cuprite_DRModel.ae
          -feat value_0 value_1 value_2 value_3 value_4 value_5 value_6 value_7 value_8 value_9
          -algorithm autoencoder
          -algorithm.autoencoder.nbneuron 8
          -algorithm.autoencoder.regularization 0.01
          -algorithm.autoencoder.noise 0
          -algorithm.autoencoder.rho 0
          -algorithm.autoencoder.beta 0)
