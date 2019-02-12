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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Simulation)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

# ------- ProsailModelExampleTest ----------

#OTBSimulation -> OTBOpticalCalibration -> OTB6S
if(OTB6S_LOADED)
otb_add_test(NAME siTvProsailModelExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-ascii ${EPSILON_3}
    ${BASELINE}/siTvProsailModelExampleTest.txt
    ${TEMP}/siTvProsailModelExampleTest.txt
  Execute $<TARGET_FILE:ProsailModel>
    30.0 #cab
    10.0 #car
    0.0 #CBrown
    0.015 #Cw
    0.009 #Cm
    1.2 #N
    2 #LAI
    50 #Angl
    1 #PSoil
    70 #Skyl
    0.2 #HSpot
    30 #TTS
    0 #TTO
    0 #PSI
    ${TEMP}/siTvProsailModelExampleTest.txt
)

otb_add_test(NAME siTvLAIAndPROSAILToSensorResponseExampleTest  COMMAND ${OTB_TEST_DRIVER}
--compare-image ${EPSILON_8}
  ${OTBAPP_BASELINE}/siTvLAIAndPROSAILToSensorResponseSmall.tif
  ${TEMP}/siTvLAIAndPROSAILToSensorResponseSmall.tif
Execute $<TARGET_FILE:LAIAndPROSAILToSensorResponse>
  ${OTB_DATA_ROOT}/Input/LAIveryverySmallFSATSW.tif
  ${TEMP}/siTvLAIAndPROSAILToSensorResponseSmall.tif
  ${OTB_DATA_ROOT}/Input/Simu_label_LAI_small.png
  ${OTB_DATA_ROOT}/Input/Simu_mask_LAI_0_small.png
  ${OTB_DATA_ROOT}/Input/label-params-SO-2006-Level-2.txt
  ${OTB_DATA_ROOT}/Input/acqui-params.txt
  ${OTB_DATA_ROOT}/Input/Radiometry/FORMOSAT2/RSI/rep6S.dat
  5
  ${OTB_DATA_ROOT}/Input/
)

endif()

otb_add_test(NAME siTvLAIFromNDVIImageTransformExampleTest  COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_8}
    ${OTB_DATA_ROOT}/Baseline/OTB/Images/raRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
    ${TEMP}/siTvLAIFromNDVIImageTransformExampleTest_verySmallFSATSW.tif
  Execute $<TARGET_FILE:LAIFromNDVIImageTransform>
    ${INPUTDATA}/verySmallFSATSW.tif
    ${TEMP}/siTvLAIFromNDVIImageTransformExampleTest_verySmallFSATSW.tif
    ${TEMP}/verySmallFSATSW_visu.png
    ${TEMP}/siTvLAIFromNDVIImageTransformExampleTest_verySmallFSATSW_visu.png
    1  # red
    4  # nir
)
