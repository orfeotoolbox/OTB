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

set(DOCUMENTATION "Map projections describe the link between geographic
coordinates and cartographic ones. So map projections allow representing a 2-dimensional
manifold of a 3-dimensional space (the Earth surface) in a 2-dimensional space
(a map which used to be a sheet of paper!). This module provides classes
generically related to this purpose. for instance, user can handle OGR data type, find RPC
solvers, re-project input vector data over input images, and so on.")

otb_module(OTBProjection
ENABLE_SHARED
  DEPENDS
    OTBBoostAdapters
    OTBCommon
    OTBGDAL
    OTBGdalAdapters
    OTBITK
    OTBImageBase
    OTBImageManipulation
    OTBMetadata
    OTBOSSIMAdapters
    OTBTransform
    OTBVectorDataBase
    OTBVectorDataManipulation
    OTBFunctor

  TEST_DEPENDS
    OTBImageIO
    OTBTestKernel
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
