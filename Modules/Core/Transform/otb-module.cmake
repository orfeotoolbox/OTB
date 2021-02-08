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

set(DOCUMENTATION "This module contains additional geometric transformations
that are more suitable to remote sensing (compared to the ones already available in
itk -see itk::Transform-). this mainly concerns transformations of coordinates
from any map projection or sensor model to any map projection or sensor model.")

otb_module(OTBTransform
  DEPENDS
    OTBBoost
    OTBCommon
    OTBGDAL
    OTBITK
    OTBInterpolation
    OTBOSSIMAdapters
    OTBMetadata
    OTBGdalAdapters
    OTBIOGDAL
  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBImageManipulation
    OTBOssim
    OTBTestKernel
    OTBProjection

  DESCRIPTION
    "${DOCUMENTATION}"
)
