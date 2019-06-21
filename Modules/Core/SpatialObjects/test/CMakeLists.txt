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

set(OTBSpatialObjectsTests
otbSpatialObjectsTestDriver.cxx
otbDrawLineSpatialObject.cxx
otbLineSpatialObjectList.cxx
otbDrawLineSpatialObjectList.cxx
)

add_executable(otbSpatialObjectsTestDriver ${OTBSpatialObjectsTests})
target_link_libraries(otbSpatialObjectsTestDriver ${OTBSpatialObjects-Test_LIBRARIES})
otb_module_target_label(otbSpatialObjectsTestDriver)


otb_add_test(NAME coTuDrawLineSpatialObject COMMAND otbSpatialObjectsTestDriver
  otbDrawLineSpatialObject
  ${INPUTDATA}/ImageLineDir.bsq
  ${TEMP}/coFiltreDrawLine_ImageLineDir.png 10. 22. 38. 42.)

otb_add_test(NAME coTuLineSpatialObjectList COMMAND otbSpatialObjectsTestDriver
  otbLineSpatialObjectList
  )

otb_add_test(NAME coTuDrawLineSpatialObjectList COMMAND otbSpatialObjectsTestDriver
  otbDrawLineSpatialObjectList
  ${INPUTDATA}/ImageLineDir.bsq
  ${TEMP}/coFiltreDrawLineList_ImageLineDir.png )
