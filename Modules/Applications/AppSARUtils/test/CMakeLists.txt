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
