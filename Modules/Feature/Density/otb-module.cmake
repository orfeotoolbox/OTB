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

set(DOCUMENTATION "This module contains tools to computes a local density of
keypoints (SIFT or SURF, for instance). Many estimators are implemented
(Epanechnikov / simple cut off / gaussian function, ratio of the number of keypoints over
the total number of pixels within a region). Note that the estimated density can be
represented as an image or quiered at particular points (function).")

otb_module(OTBDensity
  DEPENDS
    OTBPointSet
    OTBCommon
    OTBITK

  TEST_DEPENDS
    OTBDescriptors
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
