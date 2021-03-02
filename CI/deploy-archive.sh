#!/bin/bash
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

jobs_directory=/home/otbpush/test

build_dir=$(readlink -f "$1")

scp ${build_dir}/SuperBuild-archives-*.{tar.bz2,md5} otbpush@otb5-vm2.orfeo-toolbox.org:${jobs_directory}/.

ssh otbpush@otb5-vm2.orfeo-toolbox.org \
  mv ${jobs_directory}/SuperBuild-archives-*.{tar.bz2,md5} ${jobs_directory}/superbuild_archive