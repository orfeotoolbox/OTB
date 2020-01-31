#!/usr/bin/env bash
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
# This script aims to identify the unreferenced authors to invite the team to
# check if they have already signed the contributor license agreement (CLA).
# When this is done, the author must be added to the reference list.

source /build/anaconda3/bin/activate

cd /build/staged-recipes/recipes/

conda config --set anaconda_upload yes
anaconda login --username ${ANACONDA_LOGIN} --password ${ANACONDA_PASSWORD} --hostname ${CI_JOB_ID}

conda build ${CONDA_BUILD_OPTIONS} muparserx
conda build ${CONDA_BUILD_OPTIONS} openthreads
conda build ${CONDA_BUILD_OPTIONS} ossim
conda build ${CONDA_BUILD_OPTIONS} shark
conda build ${CONDA_BUILD_OPTIONS} tinyxml
conda build ${CONDA_BUILD_OPTIONS} libitk
conda build ${CONDA_BUILD_OPTIONS} otb
