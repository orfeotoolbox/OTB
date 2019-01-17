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

set(DOCUMENTATION "This module contains classes related to statistical
calculation. One part of them is dedicated to the handling of ListSample (measurements
stored in a list type structure -as opposed to -). For instance, it is possible to
concatenate several sample lists into a single one, to generate a noised version
of the input sample list, and so on. The other part works on images and labelMaps
and allows the calculation of classical statistics (min max mean histogram).")

otb_module(OTBStatistics
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase
    OTBObjectList
    OTBProjection
    OTBStreaming

  TEST_DEPENDS
    OTBCommandLineParser
    OTBImageIO
    OTBImageManipulation
    OTBInterpolation
    OTBTestKernel
    OTBVectorDataBase
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
