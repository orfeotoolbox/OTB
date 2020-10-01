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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/BasicFilters)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)

otb_add_test(NAME bfTeLeeImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/GomaSmallLeeFiltered.tif
    ${TEMP}/GomaSmallLeeFiltered.tif
  Execute $<TARGET_FILE:LeeImageFilter>
    ${INPUTDATA}/GomaSmall.tif
    ${TEMP}/GomaSmallLeeFiltered.tif
    3 1
)

# ------- FrostImageFilterTest----------

otb_add_test(NAME bfTeFrostImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/GomaSmallFrostFiltered.tif
    ${TEMP}/GomaSmallFrostFiltered.tif
  Execute $<TARGET_FILE:FrostImageFilter>
    ${INPUTDATA}/GomaSmall.tif
    ${TEMP}/GomaSmallFrostFiltered.tif
    5 0.1
)

# ------- DEMToRainbowExampleTest ----------

otb_add_test(NAME bfTeDEMToRainbowExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/DEMToRainbowImageGenerator.png
    ${TEMP}/DEMToRainbowImageGenerator.png
  Execute $<TARGET_FILE:DEMToRainbowExample>
    ${TEMP}/DEMToRainbowImageGenerator.png
    6.5 45.5 500 500 0.002 -0.002 ${OTB_DATA_ROOT}/Input/DEM/srtm_directory
)


# ------- ScalingFilterExampleTest ----------

otb_add_test(NAME bfTeScalingFilterExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/QB_Toulouse_Ortho_PAN_rescaled.png
    ${TEMP}/QB_Toulouse_Ortho_PAN_rescaled.png
    ${BASELINE}/QB_Toulouse_Ortho_PAN_casted.png
    ${TEMP}/QB_Toulouse_Ortho_PAN_casted.png
  Execute $<TARGET_FILE:ScalingFilterExample>
    ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
    ${TEMP}/QB_Toulouse_Ortho_PAN_rescaled.png
    ${TEMP}/QB_Toulouse_Ortho_PAN_casted.png
)

# ------- PrintableImageFilterTest----------

otb_add_test(NAME bfTePrintableImageFilterExample1Test COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/PrintableExampleOutput1.png
    ${TEMP}/PrintableExampleOutput1.png
  Execute $<TARGET_FILE:PrintableImageFilterExample>
    ${INPUTDATA}/IMAGERY_SSECH.tif
    ${TEMP}/PrintableExampleOutput1.png
    1 2 3
)

otb_add_test(NAME bfTePrintableImageFilteExample2rTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/PrintableExampleOutput2.png
    ${TEMP}/PrintableExampleOutput2.png
  Execute $<TARGET_FILE:PrintableImageFilterExample>
    ${INPUTDATA}/IMAGERY_SSECH.tif
    ${TEMP}/PrintableExampleOutput2.png
    1 4 2
)

# ------- IndexedToRGBExampleTest----------

otb_add_test(NAME bfTeIndexedToRGBExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/buildingExtractionRGB.png
    ${TEMP}/buildingExtractionRGB.png
    ${BASELINE}/buildingExtractionIndexed_scaled.png
    ${TEMP}/buildingExtractionIndexed_scaled.png
  Execute $<TARGET_FILE:IndexedToRGBExample>
    ${INPUTDATA}/buildingExtractionIndexed.tif
    ${TEMP}/buildingExtractionRGB.png
    ${TEMP}/buildingExtractionIndexed_scaled.png
)
#if(OTBMathParser_LOADED)
#otb_add_test(NAME bfTeMeanShiftSegmentationFilterExampleTest COMMAND ${OTB_TEST_DRIVER}
  #--compare-n-images ${NOTOL} 4
    #${BASELINE}/MSClusteredOutput.tif
    #${TEMP}/MSClusteredOutput.tif
    #${BASELINE}/MSLabeledOutput.tif
    #${TEMP}/MSLabeledOutput.tif
    #${BASELINE}/MSLabeledOutput-pretty.png
    #${TEMP}/MSLabeledOutput-pretty.png
    #${BASELINE}/MSClusteredOutput-pretty.png
    #${TEMP}/MSClusteredOutput-pretty.png
  #Execute $<TARGET_FILE:MeanShiftSegmentationFilterExample>
    #${INPUTDATA}/ROI_QB_MUL_1.png
    #${TEMP}/MSLabeledOutput.tif
    #${TEMP}/MSClusteredOutput.tif
    #${TEMP}/MSLabeledOutput-pretty.png
    #${TEMP}/MSClusteredOutput-pretty.png
    #16
    #16
    #100
    #100
    #0.1
#)
#endif()
# ------- HillShadingExampleTest ----------

otb_add_test(NAME bfTeHillShadingExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/HillShadingExample.png
    ${TEMP}/HillShadingExample.png
    ${BASELINE}/HillShadingColorExample.png
    ${TEMP}/HillShadingColorExample.png
  Execute $<TARGET_FILE:HillShadingExample>
    ${TEMP}/HillShadingExample.png
    ${TEMP}/HillShadingColorExample.png
    6.8 45.3 500 500 0.002 -0.002 ${OTB_DATA_ROOT}/Input/DEM/srtm_directory
)
