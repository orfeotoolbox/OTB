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

set(DOCUMENTATION "The output of a descriptor algorithm is usually a short
vector of numbers which is invariant to common image transformations. Descriptors are
then compared according to some criterion in order to be matched. This module
contains different descriptors such as Fourrier-Mellin, SIFT, SURF, HOG, and so on.")

otb_module(OTBDescriptors
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase
    OTBObjectList
    OTBPointSet
    OTBTransform

  OPTIONAL_DEPENDS
    OTBSiftFast
    
  TEST_DEPENDS
    OTBImageIO
    OTBImageManipulation
    OTBInterpolation
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
