#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#Contact: Jordi Inglada  <jordi.inglada@cesbio.eu>
set(__mod_name OTBPhenology)
otb_fetch_module(${__mod_name}
  "This module implements several algorithms allowing to extract phenological
  information from time profiles. These time profiles should represent
  vegetation status as for instance NDVI, LAI, etc.
A more detailed description can be found on the project website:
https://gitlab.orfeo-toolbox.org/jinglada/phenotb
"
  "${OTB_SOURCE_DIR}/Modules/Remote/${__mod_name}"
  GIT_REPOSITORY https://gitlab.orfeo-toolbox.org/jinglada/phenotb.git
  GIT_TAG 72ba22feac301ae00869b5604c2eef17d6a3fae5
  )
