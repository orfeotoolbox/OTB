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

set(DOCUMENTATION "This module contains everything users may need handle images
: clamping, shifting, scaling, rescaling intensities, concatenating of images
into one vector image, splitting a vector image into many images, and so on. Among
nice other features, let's mention: applying a standard  (or a functor)  per
band, evaluating a ImageFunction onto a source image, generating an image from DEM
data.")

otb_module(OTBImageManipulation
  DEPENDS
    OTBBoostAdapters
    OTBCommon
    OTBITK
    OTBImageBase
    OTBMetadata
    OTBInterpolation
    OTBObjectList
    OTBStreaming
    OTBTransform
    OTBIOGDAL

  TEST_DEPENDS
    OTBDensity
    OTBImageIO
    OTBTestKernel
    OTBTextures

  DESCRIPTION
    "${DOCUMENTATION}"
)
