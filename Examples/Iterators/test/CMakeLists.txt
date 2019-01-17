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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Iterators)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

# ------- ImageRegionIteratorWithIndexTest----------

otb_add_test(NAME itTeImageRegionIteratorWithIndexTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ImageRegionIteratorWithIndexOutput.png
    ${TEMP}/ImageRegionIteratorWithIndexOutput.png
  Execute $<TARGET_FILE:ImageRegionIteratorWithIndex>
    ${INPUTDATA}/ROI_QB_MUL_2.tif
    ${TEMP}/ImageRegionIteratorWithIndexOutput.png
)

# ------- NeighborhoodIterators1Test----------

otb_add_test(NAME itTeNeighborhoodIterators1Test COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators1a.png
    ${TEMP}/NeighborhoodIterators1a.png
  Execute $<TARGET_FILE:NeighborhoodIterators1>
    ${INPUTDATA}/ROI_QB_PAN_1.tif
    ${TEMP}/NeighborhoodIterators1a.png
)

# ------- NeighborhoodIterators4Test----------

otb_add_test(NAME itTeNeighborhoodIterators4aTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators4a.png
    ${TEMP}/NeighborhoodIterators4a.png
  Execute $<TARGET_FILE:NeighborhoodIterators4>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/NeighborhoodIterators4a.png
    0
)

# ------- NeighborhoodIterators4Test----------

otb_add_test(NAME itTeNeighborhoodIterators4bTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators4b.png
    ${TEMP}/NeighborhoodIterators4b.png
  Execute $<TARGET_FILE:NeighborhoodIterators4>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/NeighborhoodIterators4b.png
    1
)

# ------- NeighborhoodIterators4Test----------

otb_add_test(NAME itTeNeighborhoodIterators4cTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators4c.png
    ${TEMP}/NeighborhoodIterators4c.png
  Execute $<TARGET_FILE:NeighborhoodIterators4>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/NeighborhoodIterators4c.png
    2
)

# ------- NeighborhoodIterators4Test----------

otb_add_test(NAME itTeNeighborhoodIterators4dTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators4d.png
    ${TEMP}/NeighborhoodIterators4d.png
  Execute $<TARGET_FILE:NeighborhoodIterators4>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/NeighborhoodIterators4d.png
    5
)

# ------- NeighborhoodIterators6Test----------

otb_add_test(NAME itTeNeighborhoodIterators6aTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators6a.png
    ${TEMP}/NeighborhoodIterators6a.png
  Execute $<TARGET_FILE:NeighborhoodIterators6>
    ${TEMP}/NeighborhoodIterators6a.png
    100 100
)

# ------- NeighborhoodIterators6Test----------

otb_add_test(NAME itTeNeighborhoodIterators6bTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators6b.png
    ${TEMP}/NeighborhoodIterators6b.png
  Execute $<TARGET_FILE:NeighborhoodIterators6>
    ${TEMP}/NeighborhoodIterators6b.png
    50 150
)

# ------- NeighborhoodIterators6Test----------

otb_add_test(NAME itTeNeighborhoodIterators6cTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators6c.png
    ${TEMP}/NeighborhoodIterators6c.png
  Execute $<TARGET_FILE:NeighborhoodIterators6>
    ${TEMP}/NeighborhoodIterators6c.png
    150 50
)

# ------- ImageRegionIteratorTest----------

otb_add_test(NAME itTeImageRegionIteratorTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ImageRegionIteratorOutput.png
    ${TEMP}/ImageRegionIteratorOutput.png
  Execute $<TARGET_FILE:ImageRegionIterator>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/ImageRegionIteratorOutput.png
    10 10 110 140
)

# ------- ShapedNeighborhoodIterators2Test----------

otb_add_test(NAME itTeShapedNeighborhoodIterators2Test COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ShapedNeighborhoodIterators1b.png
    ${TEMP}/ShapedNeighborhoodIterators1b.png
  Execute $<TARGET_FILE:ShapedNeighborhoodIterators2>
    ${INPUTDATA}/BinaryImage.png
    ${TEMP}/ShapedNeighborhoodIterators1b.png
    4
)
