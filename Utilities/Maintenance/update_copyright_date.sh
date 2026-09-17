#!/bin/bash
#
# Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
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

# Change the copyright date of all files (recursively) where this script is executed

tmp_file_name="_tmp_file_list.txt"

# remove old file
# if [[ -f "$tmp_file_name" ]]; then
#   rm "$tmp_file_name"
# end
# get all files from only OTB folders 
find CI -type f > $tmp_file_name
find CMake -type f >> $tmp_file_name
find Copyright -type f >> $tmp_file_name
find Docker -type f >> $tmp_file_name
find Documentation -type f >> $tmp_file_name
find Examples -type f >> $tmp_file_name
find Modules -type f >> $tmp_file_name
find Packaging -type f >> $tmp_file_name
find SuperBuild -type f >> $tmp_file_name
find Utilities -type f >> $tmp_file_name
find . -maxdepth 1 -type f >> $tmp_file_name
current_year=$(date -u | egrep -o "[0-9]{4}")

# for all files, update the year to current
while IFS= read -r f_path; do
  # except for this current script
  if [ "$f_path" != $0 ]; then
    sed -i "s/Copyright (C) 2005-20../Copyright (C) 2005-$current_year/g" "$f_path"
  fi
done < "$tmp_file_name"

rm $tmp_file_name
