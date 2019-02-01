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

set(DOCUMENTATION "This module contains useful tools for the task of labelling
images. It is possible to label with similar statistics using connectivity, to
include pixels in a growing region is to evaluate intensity value inside a specific
interval, or to label pixels that are connected to a seed and lie within a
neighbourhood. Remaping the labels is also possible, so that that the label numbers
are consecutive with no gaps between the label numbers used. Finally, it is also
possible to sort the labels based on the size of the object.")

otb_module(OTBLabelling
  DEPENDS
    OTBITK
    OTBImageManipulation
    OTBPointSet

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
