#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

set(DOCUMENTATION "This module is concerned with Synthetic Aperture Radar
images. The ability of radar polarimetry to obtain more information about physical
properties of the surface than single-channel SAR data has led to a wide range of
applications. Here, the user can find (for instance) classes to handle different
representations of a polarisation state, to perform polarimetry synthesis, and so
on.")

otb_module(OTBPolarimetry
  DEPENDS
    OTBCommon
    OTBITK
    OTBFunctor

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
