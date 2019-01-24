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

set(OTBMarkovTests
otbMarkovTestDriver.cxx
otbMRFEnergyFisherClassification.cxx
otbMRFSamplerRandom.cxx
otbMarkovRandomFieldFilter.cxx
otbMRFSamplerMAP.cxx
otbMRFEnergyGaussian.cxx
otbMRFOptimizerMetropolis.cxx
otbMRFEnergyPotts.cxx
otbMRFEnergyEdgeFidelity.cxx
otbMRFSamplerRandomMAP.cxx
otbMRFOptimizerICM.cxx
otbMRFEnergyGaussianClassification.cxx
)

add_executable(otbMarkovTestDriver ${OTBMarkovTests})
target_link_libraries(otbMarkovTestDriver ${OTBMarkov-Test_LIBRARIES})
otb_module_target_label(otbMarkovTestDriver)

# Tests Declaration

otb_add_test(NAME maTvMRFEnergyFisherClassification COMMAND otbMarkovTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/maTvMRFEnergyFisherClassification.txt
  ${TEMP}/maTvMRFEnergyFisherClassification.txt
  otbMRFEnergyFisherClassification
  ${TEMP}/maTvMRFEnergyFisherClassification.txt
  )

otb_add_test(NAME maTvMRFSamplerRandom COMMAND otbMarkovTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/maTvMRFSamplerRandom.txt
  ${TEMP}/maTvMRFSamplerRandom.txt
  otbMRFSamplerRandom
  ${INPUTDATA}/poupees_sub_c1.png
  ${INPUTDATA}/poupees_sub_c2.png
  ${TEMP}/maTvMRFSamplerRandom.txt
  )

otb_add_test(NAME maTvMarkovRandomFieldFilter COMMAND otbMarkovTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/maTvMarkovRandomField.png
  ${TEMP}/maTvMarkovRandomField.png
  otbMarkovRandomFieldFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/maTvMarkovRandomField.png
  1.0
  30
  1.0
  )

otb_add_test(NAME maTvMRFSamplerMAP COMMAND otbMarkovTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/maTvMRFSamplerMAP.txt
  ${TEMP}/maTvMRFSamplerMAP.txt
  otbMRFSamplerMAP
  ${INPUTDATA}/poupees_sub_c1.png
  ${INPUTDATA}/poupees_sub_c2.png
  ${TEMP}/maTvMRFSamplerMAP.txt
  )

otb_add_test(NAME maTvMRFEnergyGaussian COMMAND otbMarkovTestDriver
  otbMRFEnergyGaussian
  )

otb_add_test(NAME maTvMRFOptimizerMetropolis COMMAND otbMarkovTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/maTvMRFOptimizerMetropolis.txt
  ${TEMP}/maTvMRFOptimizerMetropolis.txt
  otbMRFOptimizerMetropolis
  ${TEMP}/maTvMRFOptimizerMetropolis.txt
  )

otb_add_test(NAME maTvMRFEnergyPotts COMMAND otbMarkovTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/maTvMRFEnergyPotts.txt
  ${TEMP}/maTvMRFEnergyPotts.txt
  otbMRFEnergyPotts
  ${TEMP}/maTvMRFEnergyPotts.txt
  )

otb_add_test(NAME maTvMRFEnergyEdgeFidelity COMMAND otbMarkovTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/maTvMRFEnergyEdgeFidelity.txt
  ${TEMP}/maTvMRFEnergyEdgeFidelity.txt
  otbMRFEnergyEdgeFidelity
  ${TEMP}/maTvMRFEnergyEdgeFidelity.txt
  )

otb_add_test(NAME maTvMRFSamplerRandomMAP COMMAND otbMarkovTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/maTvMRFSamplerRandomMAP.txt
  ${TEMP}/maTvMRFSamplerRandomMAP.txt
  otbMRFSamplerRandomMAP
  ${INPUTDATA}/poupees_sub_c1.png
  ${INPUTDATA}/poupees_sub_c2.png
  ${TEMP}/maTvMRFSamplerRandomMAP.txt
  )

otb_add_test(NAME maTuMRFOptimizerICM COMMAND otbMarkovTestDriver
  otbMRFOptimizerICM
  ${TEMP}/maTuMRFOptimizerICM.txt
  )

otb_add_test(NAME maTvMRFEnergyGaussianClassification COMMAND otbMarkovTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/maTvMRFEnergyGaussianClassification.txt
  ${TEMP}/maTvMRFEnergyGaussianClassification.txt
  otbMRFEnergyGaussianClassification
  ${TEMP}/maTvMRFEnergyGaussianClassification.txt
  )

