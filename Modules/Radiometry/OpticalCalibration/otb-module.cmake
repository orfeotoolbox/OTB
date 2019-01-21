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

set(DOCUMENTATION "Digital number to radiance correction. radiance to
refletance image conversion. atmospheric correction for TOA (top of atmosphere) to TOC
(top of canopy) reflectance estimation. correction of the adjacency effects taking
into account the neighborhood contribution.")

otb_module(OTBOpticalCalibration
ENABLE_SHARED
  DEPENDS
    OTB6S
    OTBBoostAdapters
    OTBCommon
    OTBITK
    OTBImageBase
    OTBImageManipulation
    OTBMetadata
    OTBOSSIMAdapters
    OTBObjectList

  TEST_DEPENDS
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
