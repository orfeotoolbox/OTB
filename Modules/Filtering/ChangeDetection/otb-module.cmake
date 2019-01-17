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

set(DOCUMENTATION "Change detection techniques try to detect and locate areas
which have changed between two or more observations of the same scene. These
changes can be of different types, with different origins and of different temporal
length. This module contains algorithms that can be used for different situations:
abrupt changes in an image pair, abrupt changes within an image series and a
known date, abrupt changes within an image series and an unknown date, progressive
changes within an image series.")

otb_module(OTBChangeDetection
  DEPENDS
    OTBCommon
    OTBStatistics
    OTBITK
    OTBProjection
    OTBImageManipulation

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
