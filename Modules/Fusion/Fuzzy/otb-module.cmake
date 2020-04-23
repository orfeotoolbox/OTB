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

set(DOCUMENTATION "A fuzzy variable is defined as a set of qualitative values
for a magnitude. These values are ordered, as for instance Low, Medium, High. This
module contains classes able to hold a vector where each component is the
membership value for each one of the qualitative values. it also contains classes able
to handle xml files where are stored several statistics.")

otb_module(OTBFuzzy
  DEPENDS
    OTBCommon
    OTBTinyXML
    OTBITK

  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
