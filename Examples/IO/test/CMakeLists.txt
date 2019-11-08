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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/IO)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)

# ------- ExtractROITest----------

otb_add_test(NAME ioTeExtractROITest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/ROI_IMAGERY_RGB.png
    ${TEMP}/ROI_IMAGERY_RGB.png
    ${BASELINE}/ROI_IMAGERY_MIR.png
    ${TEMP}/ROI_IMAGERY_MIR.png
  Execute $<TARGET_FILE:ExtractROI>
    ${INPUTDATA}/IMAGERY_SSECH.tif
    ${TEMP}/ROI_IMAGERY_RGB.png
    ${TEMP}/ROI_IMAGERY_MIR.png
    0 0 100 100
)

otb_add_test(NAME ioTeDEMToImageGeneratorTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_5}
    ${BASELINE}/DEMToImageGenerator.tif
    ${TEMP}/DEMToImageGenerator.tif
  Execute $<TARGET_FILE:DEMToImageGenerator>
    ${TEMP}/DEMToImageGenerator.tif
    ${TEMP}/pretty_DEMToImageGenerator.png
    6.5
    45.5
    500
    500
    0.002
    -0.002
    ${INPUTDATA}/DEM_srtm
)

otb_add_test(NAME prTeDEMHandlerExampleTest COMMAND ${OTB_TEST_DRIVER}
  Execute $<TARGET_FILE:DEMHandlerExample>
    ${INPUTDATA}/DEM_srtm
    ${INPUTDATA}/DEM_srtm/egm96.grd
    40
    8.434583
    44.647083
    383.580313671
    0.001
)
