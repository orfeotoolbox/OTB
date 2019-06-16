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

set(DOCUMENTATION "In hyperspectral imagery one pixel typically consists of a
mixture of the reflectance spectra of several materials, where the mixture
coefficients correspond to the abundances of the constituting materials. The process of
unmixing one of these 'mixed' pixels is called hyperspectral image unmixing or
simply hyperspectral unmixing. This module contains classes useful in meeting that
goal.")

otb_module(OTBUnmixing
  DEPENDS
    OTBBoost
    OTBCommon
    OTBDimensionalityReduction
    OTBITK
    OTBImageBase
    OTBStatistics
    OTBWavelet
    OTBFunctor

  TEST_DEPENDS
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
