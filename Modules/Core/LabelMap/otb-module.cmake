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

set(DOCUMENTATION "This module contains classes that allow label images
manipulation based on object attributes. They are useful for Object-Based Image Analysis
(OBIA) (analyzing images at the object level instead of working at the pixel
level).")

otb_module(OTBLabelMap
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase
    OTBMoments
    OTBVectorDataBase
    OTBVectorDataManipulation

  TEST_DEPENDS
    OTBImageIO
    OTBProjection
    OTBTestKernel
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
