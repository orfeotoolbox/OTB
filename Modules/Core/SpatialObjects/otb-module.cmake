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

set(DOCUMENTATION "Geometric shapes are represented in OTB using the ITK
spatial object hierarchy. These classes are intended to support modelling of anatomical
structures in ITK. OTB uses them in order to model cartographic elements. Using
a common basic interface, the spatial objects are capable of representing regions
of space in a variety of different ways. For example: mesh structures, image
masks, and implicit equations may be used as the underlying representation scheme.
Spatial objects are a natural data structure for communicating the results of
segmentation methods and for introducing geometrical priors in both segmentation and
registration methods.")

otb_module(OTBSpatialObjects
  DEPENDS
    OTBCommon
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
