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

#Contact: Pierre Lassalle <lassallepierre34@gmail.com> 
otb_fetch_module(otbGRM
  "This module provides the GRM OTB application to perform multiscale
region-merging segmentation on satellite images. Three local homogeneity
criteria are available: the Baatz & Schäpe criterion, the Full Lambda Schedule
criterion and the simple Euclidean Distance criterion.  This application uses
the GRM (Generic Region Merging) library which allows quickly adding a new local
homogeneity criterion. Look at the template header file: GRMSegmenterTemplate.h
to see which format you must respect to add a new criterion.

A more detailed description can be found on the project website:
http://tully.ups-tlse.fr/lassallep/grm
"
  GIT_REPOSITORY https://github.com/orfeotoolbox/GRM
  GIT_TAG 043c523df522dd31fb2dfc460ef292e6a679a81f
)
