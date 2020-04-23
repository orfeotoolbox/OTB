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

set(OTBImageNoiseTests
otbImageNoiseTestDriver.cxx
otbFrostFilter.cxx
otbLeeFilter.cxx
otbGammaMAPFilter.cxx
otbKuanFilter.cxx
)

add_executable(otbImageNoiseTestDriver ${OTBImageNoiseTests})
target_link_libraries(otbImageNoiseTestDriver ${OTBImageNoise-Test_LIBRARIES})
otb_module_target_label(otbImageNoiseTestDriver)

# Tests Declaration

otb_add_test(NAME bfTvFrostFilter COMMAND otbImageNoiseTestDriver
  --compare-image ${EPSILON_7}  ${BASELINE}/bfFiltreFrost_poupees_05_05_01.tif
  ${TEMP}/bfFiltreFrost_poupees_05_05_01.tif
  otbFrostFilter
  ${INPUTDATA}/GomaAvant.tif   #poupees.hdr
  ${TEMP}/bfFiltreFrost_poupees_05_05_01.tif
  05 05 0.1)

otb_add_test(NAME bfTvFiltreLee1CanalPoupees COMMAND otbImageNoiseTestDriver
  --compare-image ${EPSILON_7}  ${BASELINE}/bfFiltreLee_05_05_04.tif
  ${TEMP}/bfFiltreLee_05_05_04.tif
  otbLeeFilter
  ${INPUTDATA}/GomaAvant.tif                  #poupees_1canal.c1.hdr
  ${TEMP}/bfFiltreLee_05_05_04.tif
  05 05 4.0)

otb_add_test(NAME bfTvFiltreLee COMMAND otbImageNoiseTestDriver
  --compare-image ${EPSILON_7}  ${BASELINE}/bfFiltreLee_05_05_12.tif
  ${TEMP}/bfFiltreLee_05_05_12.tif
  otbLeeFilter
  ${INPUTDATA}/GomaAvant.tif    #poupees.hdr
  ${TEMP}/bfFiltreLee_05_05_12.tif
  05 05 12.0)
  
  
otb_add_test(NAME bfTvFiltreGammaMAP COMMAND otbImageNoiseTestDriver
  --compare-image ${EPSILON_7}  ${BASELINE}/bfFiltreGammaMAP_05_05_12.tif
  ${TEMP}/bfFiltreGammaMAP_05_05_12.tif
  otbGammaMAPFilter
  ${INPUTDATA}/GomaAvant.tif    #poupees.hdr
  ${TEMP}/bfFiltreGammaMAP_05_05_12.tif
  05 05 12.0)  
  
otb_add_test(NAME bfTvFiltreKuan COMMAND otbImageNoiseTestDriver
  --compare-image ${EPSILON_7}  ${BASELINE}/bfFiltreKuan_05_05_12.tif
  ${TEMP}/bfFiltreKuan_05_05_12.tif
  otbKuanFilter
  ${INPUTDATA}/GomaAvant.tif    #poupees.hdr
  ${TEMP}/bfFiltreKuan_05_05_12.tif
  05 05 12.0)  
  

