#
# Copyright (C) 2017 CS SI
#
#  This file is part of Orfeo Toolbox
#
#    https://www.orfeo-toolbox.org/
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


set(OTBAppStereo_LINK_LIBS
  ${OTBStereo_LIBRARIES}
  ${OTBStatistics_LIBRARIES}
  ${OTBImageIO_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBInterpolation_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBDisplacementField_LIBRARIES}
  ${OTBDisparityMap_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBMathParser_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
)

otb_create_application(
  NAME           CostVolumeFilter
  SOURCES        otbCostVolumeFilter.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})


otb_create_application(
  NAME           PatchMatch
  SOURCES        otbPatchMatch.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})


