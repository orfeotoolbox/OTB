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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/MultiScale)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

# ------- MorphologicalPyramidAnalysisFilterExampleTest ----------

otb_add_test(NAME msTeMorphologicalPyramidAnalysisFilterExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 20
    ${BASELINE}/suburb2_an_1.jpeg ${TEMP}/suburb2_an_1.jpeg
    ${BASELINE}/suburb2_an_2.jpeg ${TEMP}/suburb2_an_2.jpeg
    ${BASELINE}/suburb2_an_3.jpeg ${TEMP}/suburb2_an_3.jpeg
    ${BASELINE}/suburb2_an_4.jpeg ${TEMP}/suburb2_an_4.jpeg
    ${BASELINE}/suburb2_sf_1.jpeg ${TEMP}/suburb2_sf_1.jpeg
    ${BASELINE}/suburb2_sf_2.jpeg ${TEMP}/suburb2_sf_2.jpeg
    ${BASELINE}/suburb2_sf_3.jpeg ${TEMP}/suburb2_sf_3.jpeg
    ${BASELINE}/suburb2_sf_4.jpeg ${TEMP}/suburb2_sf_4.jpeg
    ${BASELINE}/suburb2_if_1.jpeg ${TEMP}/suburb2_if_1.jpeg
    ${BASELINE}/suburb2_if_2.jpeg ${TEMP}/suburb2_if_2.jpeg
    ${BASELINE}/suburb2_if_3.jpeg ${TEMP}/suburb2_if_3.jpeg
    ${BASELINE}/suburb2_if_4.jpeg ${TEMP}/suburb2_if_4.jpeg
    ${BASELINE}/suburb2_id_1.jpeg ${TEMP}/suburb2_id_1.jpeg
    ${BASELINE}/suburb2_id_2.jpeg ${TEMP}/suburb2_id_2.jpeg
    ${BASELINE}/suburb2_id_3.jpeg ${TEMP}/suburb2_id_3.jpeg
    ${BASELINE}/suburb2_id_4.jpeg ${TEMP}/suburb2_id_4.jpeg
    ${BASELINE}/suburb2_sd_1.jpeg ${TEMP}/suburb2_sd_1.jpeg
    ${BASELINE}/suburb2_sd_2.jpeg ${TEMP}/suburb2_sd_2.jpeg
    ${BASELINE}/suburb2_sd_3.jpeg ${TEMP}/suburb2_sd_3.jpeg
    ${BASELINE}/suburb2_sd_4.jpeg ${TEMP}/suburb2_sd_4.jpeg
  Execute $<TARGET_FILE:MorphologicalPyramidAnalysisFilterExample>
    ${INPUTDATA}/suburb2.jpeg
    ${TEMP}/suburb2_an_1.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_an_2.jpeg?&gdal:co:QUALITY=95
    ${TEMP}/suburb2_an_3.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_an_4.jpeg?&gdal:co:QUALITY=95
    ${TEMP}/suburb2_sf_1.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_sf_2.jpeg?&gdal:co:QUALITY=95
    ${TEMP}/suburb2_sf_3.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_sf_4.jpeg?&gdal:co:QUALITY=95
    ${TEMP}/suburb2_if_1.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_if_2.jpeg?&gdal:co:QUALITY=95
    ${TEMP}/suburb2_if_3.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_if_4.jpeg?&gdal:co:QUALITY=95
    ${TEMP}/suburb2_id_1.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_id_2.jpeg?&gdal:co:QUALITY=95
    ${TEMP}/suburb2_id_3.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_id_4.jpeg?&gdal:co:QUALITY=95
    ${TEMP}/suburb2_sd_1.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_sd_2.jpeg?&gdal:co:QUALITY=95
    ${TEMP}/suburb2_sd_3.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_sd_4.jpeg?&gdal:co:QUALITY=95
    4 2
)

# ------- MorphologicalPyramidSynthesisFilterExampleTest ----------

otb_add_test(NAME msTeMorphologicalPyramidSynthesisFilterExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/suburb2_synthesis.jpeg
    ${TEMP}/suburb2_synthesis.jpeg
  Execute $<TARGET_FILE:MorphologicalPyramidSynthesisFilterExample>
    ${INPUTDATA}/suburb2.jpeg
    ${TEMP}/suburb2_synthesis.jpeg?&gdal:co:QUALITY=95
    4 2
)

# ------- MorphologicalPyramidSegmenterExampleTest ----------

otb_add_test(NAME msTeMorphologicalPyramidSegmenterExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/outputPyrSegmented.tif
    ${TEMP}/outputPyrSegmented.tif
  Execute $<TARGET_FILE:MorphologicalPyramidSegmenterExample>
    ${INPUTDATA}/msPyrMRToMS_IKO_Halles_4_2_sf_full.tif
    ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
    ${TEMP}/outputPyrSegmented.tif
    0 0.9 0.9 10
)
