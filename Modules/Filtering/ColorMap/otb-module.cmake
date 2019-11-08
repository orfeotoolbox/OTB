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

set(DOCUMENTATION "In some situation, it is desirable to represent a gray level
image in color for easier interpretation. This is particularly the case if pixel
values in the image are used to represent some data such as elevation,
deformation map, interferogram. In this case, it is important to ensure that similar
values will get similar colors. This module contains classes that can meet these
requirements.")

otb_module(OTBColorMap
  DEPENDS
    OTBITK

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
