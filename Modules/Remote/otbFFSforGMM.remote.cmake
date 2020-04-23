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

#Contact: Adrien Lagrange <adrien.lagrange@irit.fr>
otb_fetch_module(OTBFFSforGMM
  "This module implements a method to perform a fast forward feature selection using a Gaussian Mixture Model. 
A more detailed description can be found on the project website:
https://github.com/Laadr/otbFFSforGMM
"
  GIT_REPOSITORY https://github.com/Laadr/otbFFSforGMM.git
  GIT_TAG d9c31cf51b59a4a10e4daa2858a85cd12561c696
  )
