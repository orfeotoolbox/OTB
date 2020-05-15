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

set(DOCUMENTATION "This module is related to the handling of paths. For
instance, it contains the base class 'otbImageToPathFilter' for filters taking an image
as input, and producing an output of type Path. another interesting class is
'otbPathFunction', which is the base class for all objects that evaluate a function
of an image using a path list (computation of histograms for instance). And so on.")

otb_module(OTBPath
  DEPENDS
    OTBVectorDataBase
    OTBImageBase
    OTBObjectList
    OTBITK
    OTBCommon

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
