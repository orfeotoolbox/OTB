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

set(DOCUMENTATION "The region connection calculus (RCC) is used for qualitative
spatial representation and . RCC abstractly describes regions by their possible
relations to each other (8 basic relations are possible between two regions). The
RCC8 constraint language has been popularly adopted by the GIS communities. This
module provides classes related to this purpose.")

otb_module(OTBRCC8
  DEPENDS
    OTBITK
    OTBImageBase
    OTBRoadExtraction
    OTBImageManipulation
    OTBPath
    OTBCommon
    OTBBoostAdapters
    OTBObjectList

  TEST_DEPENDS
    OTBVectorDataBase
    OTBTestKernel
    OTBImageIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
