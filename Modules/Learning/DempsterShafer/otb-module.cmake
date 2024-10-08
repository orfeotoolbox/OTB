#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

set(DOCUMENTATION "Data fusion needs a common theoretical framework to
manipulate and combine data sources having different natures. The mathematical theory of
dempster-Shafer, which is based on belief function and plausible reasoning, can
be used for this purpose. This module contains classes related to this theory (see
also majority voting module).")

otb_module(OTBDempsterShafer
  DEPENDS
    OTBVectorDataBase
    OTBITK
    OTBImageBase
    OTBVectorDataManipulation
    OTBSupervised
    OTBCommon
    OTBFuzzy
    OTBMathParser
    OTBMetadata

  TEST_DEPENDS
    OTBVectorDataIO
    OTBTestKernel
    OTBImageIO
    OTBObjectList

  DESCRIPTION
    "${DOCUMENTATION}"
  
  COMPONENT
    Learning
)
