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

set(DOCUMENTATION "This module contains tools related to geometric disparities
between images. Following functionalities are implemented: estimation,
regularization (median filter), 3D points projection, production of DEM at regular sampling
in the chosen map projection system, resampling of a disparity map computed in
epipolar geometry into a disparity map relative to sensor geometry, sub-pixel
precision.")

otb_module(OTBDisparityMap
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase
    OTBPointSet
    OTBStereo
    OTBTransform

  TEST_DEPENDS
    OTBImageIO
    OTBObjectList
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
