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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/OBIA)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

otb_add_test(NAME  trTeOBIAImageToLabelToImageTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/OBIAI2L2IOutput.tif
    ${TEMP}/OBIAI2L2IOutput.tif
  Execute $<TARGET_FILE:ImageToLabelToImage>
    ${INPUTDATA}/MSLabeledOutput.tif
    ${TEMP}/OBIAI2L2IOutput.tif
    ${TEMP}/OBIAI2L2IInPretty.png
    ${TEMP}/OBIAI2L2IOutPretty.png
    1 1108 0
)

otb_add_test(NAME trTeOBIAShapeAttributeComputationTest COMMAND ${OTB_TEST_DRIVER}
  --compare-ascii ${EPSILON_3}
    ${BASELINE}/OBIAShapeAttribute.txt
    ${TEMP}/OBIAShapeAttribute.txt
  Execute $<TARGET_FILE:ShapeAttributeComputation>
    ${INPUTDATA}/MSLabeledOutput.tif
    ${TEMP}/OBIAShapeAttribute.txt
)

#if(OTBMathParser_LOADED)
#otb_add_test(NAME trTeOBIARadiometricAttributesLabelMapFilterTest COMMAND ${OTB_TEST_DRIVER}
  #--compare-image ${NOTOL}
    #${BASELINE}/OBIARadiometricAttributes.png
    #${TEMP}/OBIARadiometricAttributes.png
  #Execute $<TARGET_FILE:RadiometricAttributesLabelMapFilterExample>
    #${INPUTDATA}/qb_RoadExtract.tif
    #${TEMP}/OBIARadiometricAttributes.png
    #${TEMP}/qb_RoadExtract_Radiometry_pretty.png
    #STATS::Band1::Mean
    #0
    #0.5
    #16
    #16
    #50
    #1.0
#)
#endif()

otb_add_test(NAME trTeLabelMapToVectorData COMMAND ${OTB_TEST_DRIVER}
  Execute $<TARGET_FILE:LabelMapToVectorData>
    ${INPUTDATA}/rcc8_mire1.png
    ${TEMP}/rcc8_mire2_vectorizer.shp
)
