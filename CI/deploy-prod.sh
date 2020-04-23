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

# This script aims at moving and renaming source, packages and documentation from
# staging area to production area

staging_area=/home/otbpush/test/staging
#check if we are dealing with the right packages

pack_suffix=-$(echo "$CI_COMMIT_TAG" | grep -o "rc[0-9]*") # this retrieve the rc number
echo "pack_suffix=${pack_suffix}"

echo "Renaming binary packages"
# find build_packages/. -name "*.run" \
# -exec sh -c 'mv "$1" "${1%.run}${pack_suffix}.run"' _ {} \;
packages_list=$(ls ${staging_area} | grep -E "OTB-[0-9]*.[0-9]*.[0-9]*-.*\.(run|zip)")
ls -all build_packages/
for name in $packages_list
  do
  len=${#name}
  #add pack_suffix to the name
  mv "$name" "${name:0:$len-4}${pack_suffix}${name:$len-4}"
done

# TO REMOVE
###########
ls -all build_packages/
###########
