#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

project(OTBMapnik)

set(OTBMapnik_SYSTEM_INCLUDE_DIRS
  ${MAPNIK_INCLUDE_DIRS}
  ${ICUUC_INCLUDE_DIRS}
  ${LTDL_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  ${AGG_INCLUDE_DIRS}
  )

set(OTBMapnik_LIBRARIES
  ${MAPNIK_LIBRARIES}
  ${ICUUC_LIBRARIES}
  ${LTDL_LIBRARIES}
  ${FREETYPE_LIBRARIES}  
  )

otb_module_impl()
