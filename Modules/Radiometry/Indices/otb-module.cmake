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

set(DOCUMENTATION "With multispectral sensors, several indices can be computed
combining several spectral bands to show features that are not obvious using only
one band. This module contains filters that can compute classical indices, such
as NDVI (Normalized Difference Vegetation Index), NDWI (Normalized
DifferenceWater Index) and so on.")

otb_module(OTBIndices
  DEPENDS
    OTBCommon
    OTBFuzzy
    OTBITK
    OTBImageManipulation
    OTBMetadata
    OTBPath
    OTBVectorDataBase

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBObjectList
    OTBProjection
    OTBTestKernel
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
