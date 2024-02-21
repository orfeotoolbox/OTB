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
# Contact: Thierry Koleck  <thierry.koleck@cnes.fr>

set(mod_name S1TilingSupportApplications)
otb_fetch_module(${mod_name}
"Speckle filtering of a time-serie of SAR images using the multichanel Quegan-like filter
A more detailed description can be found on the project website:
https://gitlab.orfeo-toolbox.org/s1-tiling/s1tilingsupportapplications
"
  "${CMAKE_CURRENT_SOURCE_DIR}/${mod_name}"
  GIT_REPOSITORY https://gitlab.orfeo-toolbox.org/s1-tiling/s1tilingsupportapplications
  # Commit on master branch
  GIT_TAG 2.0.0
)
