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

set(DOCUMENTATION "This module deals with image simulation algorithm. Using
objects transmittance and reflectance and sensor characteristics, it can be possible
to generate realistic hyperspectral synthetic set of data. This module includes
PROSPECT (leaf optical properties) and SAIL (canopy bidirectional reflectance)
models, as well as PROSAIL, which is the combination of the two previous ones.")

otb_module(OTBSimulation
ENABLE_SHARED
  DEPENDS
    OTBStatistics
    OTBITK
    OTBMetadata
    OTBImageBase
    OTBLabelMap
    OTBImageManipulation
    OTBIndices
    OTBCommon
    OTBInterpolation
    OTBBoostAdapters
    OTBOpticalCalibration
    OTBObjectList

  TEST_DEPENDS
    OTBVectorDataBase
    OTBConversion
    OTBImageIO
    OTBTestKernel
    OTBLearningBase
    OTBSupervised
    OTBLibSVM
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
