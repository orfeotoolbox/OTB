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

set(OTBImageManipulation_SRC
  otbStreamingShrinkImageFilter.cxx
  )

add_library(OTBImageManipulation ${OTBImageManipulation_SRC})
target_link_libraries(OTBImageManipulation
  ${OTBCommon_LIBRARIES}
  ${OTBITK_LIBRARIES}
  ${OTBITKPendingPatches_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBInterpolation_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  ${OTBStreaming_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  )

otb_module_target(OTBImageManipulation)
