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

set(DOCUMENTATION "Most of the fusion methods in the remote sensing community
deal with the pansharpening technique. This fusion combines the image from the
PANchromatic sensor of one satellite (high spatial resolution data) with the
multispectral (XS) data (lower resolution in several spectral bands) to generate images
with a high resolution and several spectral bands. This module provides classes
related to this purpose.")

otb_module(OTBPanSharpening
  DEPENDS
    OTBImageBase
    OTBStatistics
    OTBITK
    OTBConvolution
    OTBImageManipulation
    OTBFunctor
  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
