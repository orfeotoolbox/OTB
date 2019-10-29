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

set(DOCUMENTATION "Geometric moments are widely used in image processing and
pattern recognition. This module contains classes that implement many moments: Hu,
Flusser, and so on. Note that some of them can be evaluated along a path, instead
of an image.")

otb_module(OTBMoments
  DEPENDS
    OTBCommon
    OTBITK
    OTBPath

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBImageManipulation
    OTBInterpolation
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
