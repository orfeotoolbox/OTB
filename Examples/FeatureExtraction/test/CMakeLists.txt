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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/FeatureExtraction)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)

# ------- CorrelationLineDetectorExampleTest----------

otb_add_test(NAME feTeCorrelationLineDetectorExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/amstLineCorrelations.png
    ${TEMP}/amstLineCorrelations.png
    ${BASELINE}/amstLineCorrelationDirections.png
    ${TEMP}/amstLineCorrelationDirections.png
  Execute $<TARGET_FILE:CorrelationLineDetectorExample>
    ${INPUTDATA}/amst2.png
    ${TEMP}/amstLineCorrelations.png
    ${TEMP}/amstLineCorrelationDirections.png
    5 1
)

# ------- HarrisExampleTest----------

otb_add_test(NAME feTeHarrisExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ROISpot5Harris.png
    ${TEMP}/ROISpot5Harris.png
  Execute $<TARGET_FILE:HarrisExample>
    ${INPUTDATA}/ROISpot5.png
    ${TEMP}/ROISpot5Harris.png
    1.5 2 0.1
)

# ------- TouziEdgeDetectorExampleTest----------

otb_add_test(NAME feTeTouziEdgeDetectorExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/amstTouziEdges.png
    ${TEMP}/amstTouziEdges.png
    ${BASELINE}/amstTouziDirections.png
    ${TEMP}/amstTouziDirections.png
  Execute $<TARGET_FILE:TouziEdgeDetectorExample>
    ${INPUTDATA}/amst2.png
    ${TEMP}/amstTouziEdges.png
    ${TEMP}/amstTouziDirections.png
    3
)

# ------- AsymmetricFusionOfLineDetectorExampleTest----------

otb_add_test(NAME feTeAsymmetricFusionOfLineDetectorExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/amstLineFusion.png
    ${TEMP}/amstLineFusion.png
  Execute $<TARGET_FILE:AsymmetricFusionOfLineDetectorExample>
    ${INPUTDATA}/amst2.png
    ${TEMP}/amstLineFusion.png
    5 1
)

# ------- RatioLineDetectorExampleTest----------

otb_add_test(NAME feTeRatioLineDetectorExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/amstLineRatios.png
    ${TEMP}/amstLineRatios.png
    ${BASELINE}/amstLineRatioDirections.png
    ${TEMP}/amstLineRatioDirections.png
  Execute $<TARGET_FILE:RatioLineDetectorExample>
    ${INPUTDATA}/amst2.png
    ${TEMP}/amstLineRatios.png
    ${TEMP}/amstLineRatioDirections.png
    5 1
)


#otb_add_test(NAME feTeImageToSIFTKeyPointSetFilterTest COMMAND ${OTB_TEST_DRIVER}
#        --compare-image ${NOTOL}
#        ${BASELINE}/ImageToSIFTKeyPointSetFilterOutput.png
#        ${TEMP}/ImageToSIFTKeyPointSetFilterOutput.png
#        Execute $<TARGET_FILE:ImageToSIFTKeyPointSetFilter>
#        ${INPUTDATA}/Scene.png
#        ${TEMP}/ImageToSIFTKeyPointSetFilterOutput.png
#        6 3 0.08 10
#)


# ------- CloudDetectionExampleTest----------

otb_add_test(NAME feTeCloudDetectionExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/CloudDetectionOutput.png
    ${TEMP}/PrettyCloudDetectionOutput.png
  Execute $<TARGET_FILE:CloudDetectionExample>
    ${INPUTDATA}/CloudsOnReunion.tif
    ${TEMP}/CloudDetectionOutput.tif
    ${TEMP}/PrettyCloudDetectionInput.png
    ${TEMP}/PrettyCloudDetectionOutput.png
    553 467 734 581 0.4 0.6 1.0
)

# ------- Line Segment Detector test----------
otb_add_test(NAME feTeLineSegmentDetectorExampleTest  COMMAND ${OTB_TEST_DRIVER}
  --compare-ogr ${NOTOL}
    ${BASELINE}/feTeLineSegmentDetectorExampleOutput.shp
    ${TEMP}/feTeLineSegmentDetectorExampleOutput.shp
  Execute $<TARGET_FILE:LineSegmentDetectorExample>
    ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
    ${TEMP}/feTeLineSegmentDetectorExampleOutput.shp
)

# ------- Right Angle Detection test----------

otb_add_test(NAME feTeRightAngleDetectionExample  COMMAND ${OTB_TEST_DRIVER}
  --compare-ogr ${NOTOL}
    ${BASELINE}/feTeRightAngleDetectionExampleOutput.shp
    ${TEMP}/feTeRightAngleDetectionExampleOutput.shp
  Execute $<TARGET_FILE:RightAngleDetectionExample>
    ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
    ${TEMP}/feTeRightAngleDetectionExampleOutput.shp
    0.1
    10.
)
