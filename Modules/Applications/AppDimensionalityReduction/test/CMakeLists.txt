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

