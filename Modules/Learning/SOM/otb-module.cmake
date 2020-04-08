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

set(DOCUMENTATION "The Self Organizing Map, SOM, introduced by Kohonen is a
non-supervised neural learning algorithm. The map is composed of neighboring cells
which are in competition by means of mutual interactions and they adapt in order
to match characteristic patterns of the examples given during the learning. The
SOM is usually on a plane (2D).")

otb_module(OTBSOM
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase

  TEST_DEPENDS
    OTBImageIO
    OTBImageManipulation
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
